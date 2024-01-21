// Dog Fight Game Code By CYM.


#include "Pawn/PlayerCharacter/CharacterAnimComponent.h"

#include "MotionWarpingComponent.h"
#include "Common/DogFightGameLog.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
UCharacterAnimComponent::UCharacterAnimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UCharacterAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get motion warping component
	MotionWarpingComponent = GetOwner()->GetComponentByClass<UMotionWarpingComponent>();

	if (auto Character = Cast<ACharacter>(GetOwner()))
	{
		AnimInstance = Character->GetMesh()->GetAnimInstance();
	}
}


// Called every frame
void UCharacterAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UCharacterAnimComponent::PlayAnimation(UAnimMontage* InAnimMontage)
{
	// Play animation across network
	MulticastPlayMontage(InAnimMontage);

	return IsValid(InAnimMontage) ? InAnimMontage->GetPlayLength() : 0.f;
}

float UCharacterAnimComponent::PlayAnimationWithWarping(UAnimMontage* InAnimMontage, FName TargetName,
	const FVector& TargetPos)
{
	// Play animation across network
	FCharacterAnimWarpingParams NewParams;
	NewParams.TargetName = TargetName;
	NewParams.TargetPos = TargetPos;
	MulticastPlayMontageWithWarping(InAnimMontage, NewParams);

	return IsValid(InAnimMontage) ? InAnimMontage->GetPlayLength() : 0.f;
}

void UCharacterAnimComponent::MulticastPlayMontage_Implementation(UAnimMontage* InMontage)
{
	if (!IsValid(InMontage))
	{
		return;
	}

	if (AnimInstance.IsValid())
	{
		const float Result = AnimInstance->Montage_Play(InMontage);
		if (Result == 0)
		{
			UE_LOG(LogDogFightGame, Error, TEXT("[UCharacterAnimComponent] Failed to play montage: %s"), *InMontage->GetName());
		}
	}
}

void UCharacterAnimComponent::MulticastPlayMontageWithWarping_Implementation(UAnimMontage* InMontage,
	const FCharacterAnimWarpingParams& WarpingParams)
{
	if (!IsValid(InMontage))
	{
		return;
	}

	// Set warping data
	if (MotionWarpingComponent.IsValid())
	{
		FMotionWarpingTarget NewTarget;
		NewTarget.Name = WarpingParams.TargetName;
		NewTarget.Location = WarpingParams.TargetPos;
		NewTarget.Rotation = GetOwner()->GetActorRotation();
		MotionWarpingComponent->AddOrUpdateWarpTarget(NewTarget);

		// Record warping data
		AnimWarpingDataMap.Add(InMontage, WarpingParams);
	}

	if (AnimInstance.IsValid())
	{
		const float Result = AnimInstance->Montage_Play(InMontage);
		if (Result == 0)
		{
			UE_LOG(LogDogFightGame, Error, TEXT("[UCharacterAnimComponent] Failed to play montage: %s"), *InMontage->GetName());
		}
		else
		{
			WaitingMontage = InMontage;
			// Listen to montage finish event
			AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &UCharacterAnimComponent::OnMontageEnded);
		}
	}
}

void UCharacterAnimComponent::OnMontageEnded(UAnimMontage* InMontage, bool bInterrupted)
{
	if (InMontage == WaitingMontage)
	{
		// Clear the warping target
		if (MotionWarpingComponent.IsValid())
		{
			if (auto WarpingDataPtr = AnimWarpingDataMap.Find(InMontage))
			{
				MotionWarpingComponent->RemoveWarpTarget(WarpingDataPtr->TargetName);
				AnimWarpingDataMap.Remove(InMontage);
			}
		}

		// Remove listener
		if (AnimInstance.IsValid())
		{
			AnimInstance->OnMontageEnded.RemoveDynamic(this, &UCharacterAnimComponent::OnMontageEnded);
		}
	}
}

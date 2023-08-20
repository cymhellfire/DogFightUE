// Dog Fight Game Code By CYM.


#include "Pawn/PlayerCharacter/CharacterAnimComponent.h"

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

	// ...
	
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

void UCharacterAnimComponent::MulticastPlayMontage_Implementation(UAnimMontage* InMontage)
{
	if (!IsValid(InMontage))
	{
		return;
	}

	if (auto Character = Cast<ACharacter>(GetOwner()))
	{
		if (auto AnimInstance = Character->GetMesh()->GetAnimInstance())
		{
			const float Result = AnimInstance->Montage_Play(InMontage);
			if (Result == 0)
			{
				UE_LOG(LogDogFightGame, Error, TEXT("[UCharacterAnimComponent] Failed to play montage: %s"), *InMontage->GetName());
			}
		}
	}
}

// Dog Fight Game Code By CYM.


#include "Animation/Notify/AnimNotifyState_FacingTarget.h"

#include "Kismet/KismetMathLibrary.h"
#include "Pawn/Component/CharacterAnimComponent.h"

UAnimNotifyState_FacingTarget::UAnimNotifyState_FacingTarget()
	: Process(0.f)
	, Duration(0.f)
{
#if WITH_EDITOR
	NotifyColor = FColor::Yellow;
#endif
}

void UAnimNotifyState_FacingTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Process = 0.f;
	// Record duration
	Duration = TotalDuration;

	SetupNotifyData(MeshComp->GetOwner(), Cast<UAnimMontage>(Animation));
}

void UAnimNotifyState_FacingTarget::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UAnimNotifyState_FacingTarget::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	Process += FrameDeltaTime;
	float Percent = FMath::Min(1.f, Process / Duration);
	if (auto TargetActor = MeshComp->GetOwner())
	{
		FRotator NewRotator = FMath::Lerp(InitialRotator, FinalRotator, Percent);
		TargetActor->SetActorRotation(NewRotator);
	}
}

void UAnimNotifyState_FacingTarget::SetupNotifyData(AActor* OwnerActor, UAnimMontage* InMontage)
{
	if (!IsValid(OwnerActor) || !IsValid(InMontage))
		return;

	auto AnimComponent = Cast<UCharacterAnimComponent>(OwnerActor->GetComponentByClass(UCharacterAnimComponent::StaticClass()));
	if (!IsValid(AnimComponent))
		return;

	if (auto Target = AnimComponent->GetAnimWarpingParams(InMontage))
	{
		InitialRotator = OwnerActor->GetActorRotation();
		// Calculate final rotator
		FinalRotator = UKismetMathLibrary::FindLookAtRotation(OwnerActor->GetActorLocation(), Target->TargetPos);
	}
}

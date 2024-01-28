// Dog Fight Game Code By CYM.


#include "Animation/Notify/AnimNotifyState_AttackDetect.h"

UAnimNotifyState_AttackDetect::UAnimNotifyState_AttackDetect()
	: UAnimNotifyState()
{
	bAttackDetectOn = false;
#if WITH_EDITOR
	NotifyColor = FColor::Red;
#endif
}

void UAnimNotifyState_AttackDetect::BeginDestroy()
{
	Super::BeginDestroy();

	// Clear delegate
	OnAttackDetectStateChanged.Clear();
}

void UAnimNotifyState_AttackDetect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!bAttackDetectOn)
	{
		// Trigger delegate
		OnAttackDetectStateChanged.Broadcast(true);

		bAttackDetectOn = true;
	}
}

void UAnimNotifyState_AttackDetect::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (bAttackDetectOn)
	{
		// Trigger delegate
		OnAttackDetectStateChanged.Broadcast(false);

		bAttackDetectOn = false;
	}
}

// Dog Fight Game Code By CYM.


#include "Animation/AnimNotify_SwitchHitDetect.h"

UAnimNotify_SwitchHitDetect::UAnimNotify_SwitchHitDetect()
{
	DamageRatio = 1.f;
}

void UAnimNotify_SwitchHitDetect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	// Broadcast delegate
	OnHitDetectSwitched.Broadcast(this, bTurnOn, DamageRatio, CollisionParentSocketName);
}

// Dog Fight Game Code By CYM.


#include "AnimNotify_Delegate.h"

UAnimNotify_Delegate::UAnimNotify_Delegate()
{
	DelegateType = EAnimNotifyDelegateType::DT_Default;
}

void UAnimNotify_Delegate::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnAnimNotifyTriggered.Broadcast(this);
}

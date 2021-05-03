// Dog Fight Game Code By CYM.


#include "Animation/AnimNotify_InvincibleFrame.h"

void UAnimNotify_InvincibleFrame::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CachedWorld = MeshComp->GetWorld();
	if (CachedWorld && !CachedWorld->IsPreviewWorld())
	{
		// Trigger begin invincible frame delegate
		OnInvincibleStateChanged.Broadcast(this, true);

		if (InvincibleDuration > 0.f)
		{
			// Register timer for invincible frame
			CachedWorld->GetTimerManager().SetTimer(InvincibleTimerHandle, this, &UAnimNotify_InvincibleFrame::OnInvincibleFrameTimerExpired, InvincibleDuration);
		}
		else
		{
			OnInvincibleFrameTimerExpired();
		}
	}
}

void UAnimNotify_InvincibleFrame::OnInvincibleFrameTimerExpired()
{
	if (CachedWorld && !CachedWorld->IsPreviewWorld())
	{
		CachedWorld->GetTimerManager().ClearTimer(InvincibleTimerHandle);

		// Trigger end invincible frame delegate
		OnInvincibleStateChanged.Broadcast(this, false);
	}
}

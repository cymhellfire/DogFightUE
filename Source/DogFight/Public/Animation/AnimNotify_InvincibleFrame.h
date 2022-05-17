// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_InvincibleFrame.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UAnimNotify_InvincibleFrame : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInvincibleFrameStateSiguature, const UAnimNotify_InvincibleFrame*, Notify, bool, bEnableInvincible);
	FInvincibleFrameStateSiguature OnInvincibleStateChanged;

protected:
	UFUNCTION()
	void OnInvincibleFrameTimerExpired();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	float InvincibleDuration;

	UWorld* CachedWorld;
	FTimerHandle InvincibleTimerHandle;
};

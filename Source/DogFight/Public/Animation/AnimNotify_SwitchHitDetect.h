// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SwitchHitDetect.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UAnimNotify_SwitchHitDetect : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_SwitchHitDetect();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHitDetectSwitchedSignature, const UAnimNotify_SwitchHitDetect*, Notify, bool, bTurnOn, float, DamageRatio, FName, ParentSocketName);
	FOnHitDetectSwitchedSignature OnHitDetectSwitched;
public:
	/** The factor used to calculated final damage apply to target. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	float DamageRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	bool bTurnOn;

	/** Specify parent socket of weapon mesh which should be used as hit collider. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	FName CollisionParentSocketName;
};

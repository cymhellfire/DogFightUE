// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapons/WeaponActionBase.h"
#include "WeaponActionDirectional.generated.h"

enum EActionDirection : uint8
{
	AD_Forward,
	AD_Backward,
	AD_Left,
	AD_Right,
};

/**
 * Weapon action that auto detect obstacle and decides direction.
 */
UCLASS()
class DOGFIGHT_API UWeaponActionDirectional : public UWeaponActionBase
{
	GENERATED_BODY()

public:
	UWeaponActionDirectional();

protected:
	virtual void PrepareActionMontage() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponActionDirectional")
	UAnimMontage* ActionMontageForward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponActionDirectional")
	UAnimMontage* ActionMontageBackward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponActionDirectional")
	UAnimMontage* ActionMontageLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponActionDirectional")
	UAnimMontage* ActionMontageRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponActionDirectional")
	float ObstacleDetectDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponActionDirectional")
	FName ObstacleDetectProfile;
};

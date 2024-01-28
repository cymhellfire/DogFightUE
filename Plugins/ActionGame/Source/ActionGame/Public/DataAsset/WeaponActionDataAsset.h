// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Common/WeaponCommon.h"
#include "Object/WeaponActionBase.h"
#include "WeaponActionDataAsset.generated.h"

class UWeaponActionBase;

USTRUCT(BlueprintType)
struct FWeaponActionTransitionSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponActionTransitionSettings")
	EWeaponActionInput InputType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponActionTransitionSettings")
	UWeaponActionDataAsset* WeaponAction;
};

/**
 * Data asset that defines various weapon actions.
 */
UCLASS(Blueprintable)
class ACTIONGAME_API UWeaponActionDataAsset : public UObject
{
	GENERATED_BODY()

public:
	UWeaponActionDataAsset()
	{
		ActionClass = UWeaponActionBase::StaticClass();
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponAction")
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponAction")
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponAction")
	TSoftObjectPtr<UAnimMontage> AnimMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponAction")
	TArray<FWeaponActionTransitionSettings> TransitionSettings;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="WeaponAction")
	TSoftClassPtr<UWeaponActionBase> ActionClass;
};

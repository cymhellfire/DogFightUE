// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WeaponBase.generated.h"

class UWeaponActionBase;
class UWeaponActionDataAsset;

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UWeaponBase : public UObject
{
	GENERATED_BODY()

public:

	virtual void AddWeaponAction(UWeaponActionDataAsset* InActionData);
protected:
	UPROPERTY(Transient)
	TArray<UWeaponActionBase*> WeaponActions;
};

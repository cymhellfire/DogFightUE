// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "WeaponCommon.h"
#include "UObject/Object.h"
#include "WeaponActionBase.generated.h"

class UWeaponActionDataAsset;
class UWeaponActionTransitionBase;
class IActionCharacterInterface;

struct FWeaponActionTarget
{
	EWeaponActionTargetType TargetType;
	union
	{
		AActor* Actor;
		FVector Location;
	} Target;

	AActor* GetActorTarget() const
	{
		if (TargetType == EWeaponActionTargetType::WATT_Actor)
		{
			return Target.Actor;
		}
		return nullptr;
	}

	FVector GetLocationTarget() const
	{
		if (TargetType == EWeaponActionTargetType::WATT_Location)
		{
			return Target.Location;
		}
		return FVector::ZeroVector;
	}
};

/**
 * Base class of all weapon actions.
 */
UCLASS()
class DOGFIGHTGAME_API UWeaponActionBase : public UObject
{
	GENERATED_BODY()

public:
	UWeaponActionBase();

	virtual void InitActionData(UWeaponActionDataAsset* InData, IActionCharacterInterface* InOwner);

	virtual void SetActionTarget(const FWeaponActionTarget& InTarget);

	virtual void Execute();

	//virtual void PerformAction();

protected:
	virtual bool CheckDistance();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction")
	FText ActionName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction")
	FText ActionDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction")
	UAnimMontage* ActionMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction")
	bool bNeedTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponAction")
	float ActionRange;

protected:
	UPROPERTY(Transient)
	TMap<EWeaponActionInput, UWeaponActionTransitionBase*> TransitionMap;

	IActionCharacterInterface* Performer;
	TOptional<FWeaponActionTarget> ActionTarget;
};

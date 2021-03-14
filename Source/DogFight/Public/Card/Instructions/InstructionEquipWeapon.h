// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Card/Instructions/HandleTargetInstructionBase.h"
#include "InstructionEquipWeapon.generated.h"

class IWeaponCarrierInterface;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInstructionEquipWeapon : public UHandleTargetInstructionBase
{
	GENERATED_BODY()

public:
	UInstructionEquipWeapon(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool HandleActorTarget(AActor* Target) override;

	UFUNCTION()
	virtual void OnWeaponEquipped(AActor* CarrierActor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	TSubclassOf<class UWeaponBase> WeaponClass;

	int32 WaitingCount;
};

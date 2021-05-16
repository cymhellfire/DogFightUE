// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Card/Instructions/HandleTargetInstructionBase.h"
#include "InstructionSpawnDamageArea.generated.h"

class ADamageArea;
class AVfxBase;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInstructionSpawnDamageArea : public UHandleTargetInstructionBase
{
	GENERATED_BODY()

public:
	UInstructionSpawnDamageArea(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void HandlePositionTarget(FVector Position) override;

	UFUNCTION()
	void OnDamageAreaFinished(ADamageArea* DamageArea);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableIntProperty Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableIntProperty DamageRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	float DamageDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableIntProperty StrengthCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	TSubclassOf<AVfxBase> VfxClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	bool bSpawnSingularityArea;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction", meta=(EditCondition="bSpawnSingularityArea"))
	float SingularityAreaMaxRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction", meta=(EditCondition="bSpawnSingularityArea"))
	float SingularityAreaMinRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction", meta=(EditCondition="bSpawnSingularityArea"))
	float SingularityDragForceSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction", meta=(EditCondition="bSpawnSingularityArea"))
	float DragForceMinRatio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction", meta=(EditCondition="bSpawnSingularityArea"))
	int32 SingularityStrengthCostPerTick;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction", meta=(EditCondition="bSpawnSingularityArea"))
	float SingularityStrengthCostTickInterval;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction", meta=(EditCondition="bSpawnSingularityArea"))
	float SingularityDuration;

protected:

	TArray<ADamageArea*> DamageAreaList;
};

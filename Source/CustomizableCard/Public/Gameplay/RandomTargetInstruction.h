// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "Gameplay/CardInstructionBase.h"
#include "RandomTargetInstruction.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMIZABLECARD_API URandomTargetInstruction : public UCardInstructionBase
{
	GENERATED_BODY()

public:
	URandomTargetInstruction(const FObjectInitializer& ObjectInitializer);

	virtual void Execute() override;

protected:

	void GenerateActorTarget();

	void GeneratePositionTarget();

	void GenerateDirectionTarget();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	ECardInstructionTargetType TargetType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	int32 TargetCount;

	/** Whether think self as an invalid Actor target. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	bool bIgnoreSelf;
};

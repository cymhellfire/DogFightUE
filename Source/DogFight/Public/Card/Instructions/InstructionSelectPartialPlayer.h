// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Card/Instructions/CardInstructionBase.h"
#include "InstructionSelectPartialPlayer.generated.h"

UENUM(BlueprintType)
enum class EPartialSelectCapMethod : uint8
{
	Floor,
	Ceil,
};

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInstructionSelectPartialPlayer : public UCardInstructionBase
{
	GENERATED_BODY()

public:
	UInstructionSelectPartialPlayer(const FObjectInitializer& ObjectInitializer);

	virtual void Execute() override;

protected:

	void GeneratePlayerTarget(int32 TargetCount);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	float SelectRatio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	EPartialSelectCapMethod SelectCountCapMethod;
};

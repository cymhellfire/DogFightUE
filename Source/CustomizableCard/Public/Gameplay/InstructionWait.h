// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "CardInstructionBase.h"
#include "UObject/ObjectMacros.h"

#include "InstructionWait.generated.h"

USTRUCT()
struct FInstructionWaitMeta : public FInstructionMetaBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Instruction")
	float WaitDuration;

	FInstructionWaitMeta()
	{
		InstructionName = TEXT("Waiting");
		WaitDuration = 1.0f;
	}
}; 

/**
 * Instruction with waiting functionality can delay other instructions behind.
 */
UCLASS()
class CUSTOMIZABLECARD_API UInstructionWait : public UCardInstructionBase
{
	GENERATED_BODY()

public:
	UInstructionWait(const FObjectInitializer& ObjectInitializer);

	virtual void Execute() override;

protected:
	/** Fired when waiting timer is expired. */
	void OnWaitingFinished();

protected:
	/** How long will this instruction stay? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Instruction");
	float WaitDuration;

	FTimerHandle WaitingTimerHandle;
};

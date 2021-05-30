// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Card/Instructions/CardInstructionBase.h"
#include "InstructionAlternativeCards.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInstructionAlternativeCards : public UCardInstructionBase
{
	GENERATED_BODY()

public:
	UInstructionAlternativeCards(const FObjectInitializer& ObjectInitializer);

	virtual void Execute() override;

protected:

	void GiveCards(int32 Count);

	UFUNCTION()
	void OnDiscardCardFinished();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableIntProperty DiscardCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FUpgradableIntProperty GainCount;

	class AStandardModePlayerController* TargetPlayerController;
	class AStandardPlayerState* TargetPlayerState;
};

// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/SlotWidget.h"
#include "OperationHintMessageWidget.generated.h"

namespace EOperationHintMessageContent
{
	extern const FName SelectActorTarget;
	extern const FName SelectPositionTarget;
	extern const FName SelectDirectionTarget;
	extern const FName DiscardCard;
	extern const FName ResponseToCard;
}

/**
 * Widget that displays a hint message to player.
 */
UCLASS()
class UOperationHintMessageWidget : public USlotWidget
{
	GENERATED_BODY()

public:
	/** Fire when displaying hint message is changed. */
	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|UI")
	void OnHintMessageChanged(const FText& HintMessage);

	void SetHintMessage(FName& NewMessage, TArray<FText> Arguments);

protected:

	FName CurrentHintMessage;
};

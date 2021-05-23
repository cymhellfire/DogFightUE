// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API USlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SlotWidget")
	FName SlotToAdd;
};

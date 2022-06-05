#pragma once

#include "CoreMinimal.h"
#include "Card.generated.h"

UCLASS(Blueprintable)
class CARDSYSTEM_API UCard : public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent)
	void Execute();
};

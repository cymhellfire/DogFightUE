#pragma once

#include "InGameMessageFunctionLibrary.generated.h"

/**
 * Provide all functions related to in-game message.
 */
UCLASS()
class DOGFIGHTGAME_API UInGameMessageFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category="InGameMessageFunction")
	static void SetTitleMessage(const FText& InText);
};

#pragma once

#include "CommonWidgetFucntionLibrary.generated.h"

UCLASS()
class DOGFIGHTGAME_API UCommonWidgetFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="CommonWidgetFunctionLibrary")
	static FSlateColor MakeSlateColor(float R, float G, float B, float A);
};

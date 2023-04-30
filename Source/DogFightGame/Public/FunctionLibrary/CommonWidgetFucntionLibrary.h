#pragma once

#include "CommonWidgetFucntionLibrary.generated.h"

class UImage;
class UAttributeWrapperObjectBase;

UCLASS()
class DOGFIGHTGAME_API UCommonWidgetFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="CommonWidgetFunctionLibrary")
	static FSlateColor MakeSlateColor(float R, float G, float B, float A);

	UFUNCTION(BlueprintCallable, Category="CommonWidgetFunctionLibrary")
	static void SetImageByTexturePath(UImage* InWidget, const FString& InPath, bool bMatchSize = false);

	UFUNCTION(BlueprintCallable, Category="CommonWidgetFunctionLibrary")
	static FText GetAttributeLocalizedName(UAttributeWrapperObjectBase* InAttrObject);
};

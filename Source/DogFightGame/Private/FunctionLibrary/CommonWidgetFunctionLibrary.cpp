#include "Components/Image.h"
#include "FunctionLibrary/CommonWidgetFucntionLibrary.h"

FSlateColor UCommonWidgetFunctionLibrary::MakeSlateColor(float R, float G, float B, float A)
{
	return FSlateColor(FLinearColor{R, G, B, A});
}

void UCommonWidgetFunctionLibrary::SetImageByTexturePath(UImage* InWidget, const FString& InPath, bool bMatchSize)
{
	if (IsValid(InWidget))
	{
		const TSoftObjectPtr<UTexture2D> TexturePtr(InPath);
		InWidget->SetBrushFromSoftTexture(TexturePtr, bMatchSize);
	}
}

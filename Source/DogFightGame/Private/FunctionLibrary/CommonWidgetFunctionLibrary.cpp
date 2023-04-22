#include "Components/Image.h"
#include "FunctionLibrary/CommonWidgetFucntionLibrary.h"
#include "GameService/GameService.h"
#include "GameService/LocalizationService.h"
#include "UnrealIntegration/DataWrapper/AttributeWrapperObject.h"

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

FText UCommonWidgetFunctionLibrary::GetAttributeLocalizedName(UAttributeWrapperObjectBase* InAttrObject)
{
	if (auto LocalizeService = UGameService::GetGameServiceBySuperClass<ULocalizationService>())
	{
		auto AttributeName = InAttrObject->GetAttributeName();
		return LocalizeService->GetLocalizeString("ST_Attribute", AttributeName.ToString());
	}

	return FText::FromString("No Localization Available.");
}


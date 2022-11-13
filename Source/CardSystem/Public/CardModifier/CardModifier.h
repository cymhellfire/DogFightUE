#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/Modifier/AttributeModifierCommon.h"
#include "UnrealIntegration/UObject/AttributeModifierBasedObject.h"
#include "CardModifier.generated.h"

class FAttributeBase;
class FAttributeModifierBase;

UCLASS(Blueprintable)
class CARDSYSTEM_API UCardModifier : public UAttributeModifierBasedObject
{
	GENERATED_BODY()
public:
	virtual void PostInitProperties() override;

	void Initialize();

	UFUNCTION(BlueprintNativeEvent, Category="CardModifier")
	FModifierCreateArgument GetModifierCreateArgument();

	EAttributeDataType GetDataType() const;
};

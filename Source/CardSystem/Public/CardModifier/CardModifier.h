#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/Modifier/AttributeModifierCommon.h"
#include "CardModifier.generated.h"

class FAttributeBase;
class FAttributeModifierBase;

UCLASS(Blueprintable)
class CARDSYSTEM_API UCardModifier : public UObject
{
	GENERATED_BODY()
public:
	virtual void PostInitProperties() override;

	void Initialize();

	UFUNCTION(BlueprintNativeEvent, Category="CardModifier")
	FModifierCreateArgument GetModifierCreateArgument();

	UFUNCTION(BlueprintNativeEvent, Category="CardModifier")
	bool GetInitialBooleanValue();

	UFUNCTION(BlueprintNativeEvent, Category="CardModifier")
	int32 GetInitialIntegerValue();

	UFUNCTION(BlueprintNativeEvent, Category="CardModifier")
	float GetInitialFloatValue();

	void ApplyToAttribute(TSharedPtr<FAttributeBase> InAttribute);
	void RemoveFromTarget();

	EAttributeDataType GetDataType() const;

protected:
	TSharedPtr<FAttributeModifierBase> Modifier;

	TWeakPtr<FAttributeBase> ModifiedAttribute;
};

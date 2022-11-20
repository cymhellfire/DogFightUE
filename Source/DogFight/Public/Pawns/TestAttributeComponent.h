#pragma once

#include "UnrealIntegration/DataWrapper/AttributeWrapper.h"
#include "UnrealIntegration/UObject/AttributeBasedComponent.h"
#include "TestAttributeComponent.generated.h"

UCLASS(BlueprintType)
class DOGFIGHT_API UTestAttributeComponent : public UAttributeBasedComponent
{
	GENERATED_BODY()
public:
	UTestAttributeComponent(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeComponent() override;

	virtual void InitializeAttributes() override;

	void ChangeTestAttribute(int32 NewValue);

	void AddTestModifier();

	void RemoveTestModifier();

protected:
	void OnTestAttributeValueChanged(TSharedPtr<FAttributeBase> Attribute);

	FTimerHandle RemoveAttributeTimer;
};

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void InitializeComponent() override;

	void ChangeTestAttribute(int32 NewValue);

	void AddTestModifier();

	void RemoveTestModifier();

protected:
	void OnTestAttributeValueChanged(TSharedPtr<FAttributeBase> Attribute);

	void SetTestInteger(int32 NewValue);

	UFUNCTION()
	void OnRep_TestInteger(const FAttributeIntegerWrapper& OldInteger);

protected:
	UPROPERTY(BlueprintReadWrite, Category="TestAttribute", ReplicatedUsing=OnRep_TestInteger)
	FAttributeIntegerWrapper TestInteger;
};

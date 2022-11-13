#pragma once

#include "UnrealIntegration/DataWrapper/UnrealAttribute.h"
#include "UnrealIntegration/Component/AttributeBasedComponent.h"
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

protected:
	void OnTestAttributeValueChanged(TSharedPtr<FAttributeBase> Attribute);

	void SetTestInteger(int32 NewValue);

	UFUNCTION()
	void OnRep_TestInteger(const FNetAttributeInteger& OldInteger);

protected:
	UPROPERTY(BlueprintReadWrite, Category="TestAttribute", ReplicatedUsing=OnRep_TestInteger)
	FNetAttributeInteger TestInteger;
};

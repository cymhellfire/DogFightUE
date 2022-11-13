#pragma once

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

protected:
	void OnTestAttributeValueChanged(TSharedPtr<FAttributeBase> Attribute);

	void SetTestInteger(int32 NewValue);

	UFUNCTION()
	void OnRep_TestInteger(int32 OldInteger);

protected:
	UPROPERTY(BlueprintReadWrite, Category="TestAttribute", ReplicatedUsing=OnRep_TestInteger)
	int32 TestInteger;
};

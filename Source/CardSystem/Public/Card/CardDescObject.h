#pragma once

#include "UnrealIntegration/UObject/AttributeBasedObject.h"
#include "CardDescObject.generated.h"

/**
 * Card description object that is replicated through network.
 */
UCLASS()
class CARDSYSTEM_API UCardDescObject : public UAttributeBasedObject
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Create wrapper struct for every created attribute
	virtual bool OnAttributeAdded(TSharedPtr<FAttributeBase> InAttribute) override;

	// Use custom wrapper getter cause all attributes of card are created at runtime.
	virtual bool UseCustomAttributeWrapperGetter() const override { return true; }

	virtual TSharedPtr<FAttributeBooleanWrapper> GetBooleanAttributeWrapperByName(FName InName) override;
	virtual TSharedPtr<FAttributeIntegerWrapper> GetIntegerAttributeWrapperByName(FName InName) override;
	virtual TSharedPtr<FAttributeFloatWrapper> GetFloatAttributeWrapperByName(FName InName) override;

	UFUNCTION()
	void OnRep_IntegerWrapperMap();

protected:
	TMap<FName, TSharedPtr<FAttributeBooleanWrapper>> BooleanWrapperMap;
	TMap<FName, TSharedPtr<FAttributeIntegerWrapper>> IntegerWrapperMap;
	TMap<FName, TSharedPtr<FAttributeFloatWrapper>> FloatWrapperMap;
};

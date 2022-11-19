#pragma once

#include "UnrealIntegration/UObject/AttributeBasedObject.h"
#include "CardDescObject.generated.h"

class UAttributeBooleanWrapperObject;
class UAttributeIntegerWrapperObject;
class UAttributeFloatWrapperObject;

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

	virtual UAttributeBooleanWrapperObject* GetBooleanAttributeWrapperByName(FName InName) override;
	virtual UAttributeIntegerWrapperObject* GetIntegerAttributeWrapperByName(FName InName) override;
	virtual UAttributeFloatWrapperObject* GetFloatAttributeWrapperByName(FName InName) override;

	UFUNCTION()
	void OnRep_BooleanWrapperList();

	UFUNCTION()
	void OnRep_IntegerWrapperList();

	UFUNCTION()
	void OnRep_FloatWrapperList();

protected:
	UPROPERTY(Transient, ReplicatedUsing=OnRep_BooleanWrapperList)
	TArray<UAttributeBooleanWrapperObject*> BooleanWrapperList;

	UPROPERTY(Transient, ReplicatedUsing=OnRep_IntegerWrapperList)
	TArray<UAttributeIntegerWrapperObject*> IntegerWrapperList;

	UPROPERTY(Transient, ReplicatedUsing=OnRep_FloatWrapperList)
	TArray<UAttributeFloatWrapperObject*> FloatWrapperList;

	// Map container that do NOT replicated. These maps are maintained by OnRep functions on client side.
	TMap<FName, TWeakObjectPtr<UAttributeBooleanWrapperObject>> BooleanWrapperMap;
	TMap<FName, TWeakObjectPtr<UAttributeIntegerWrapperObject>> IntegerWrapperMap;
	TMap<FName, TWeakObjectPtr<UAttributeFloatWrapperObject>> FloatWrapperMap;
};

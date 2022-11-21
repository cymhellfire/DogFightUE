#pragma once

#include "CoreMinimal.h"
#include "UnrealIntegration/Interface/AttributeCarrierInterface.h"
#include "AttributeBasedComponent.generated.h"

UCLASS()
class ATTRIBUTESYSTEM_API UAttributeBasedComponent : public UActorComponent, public IAttributeCarrierInterface
{
	GENERATED_BODY()
public:
	UAttributeBasedComponent(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeComponent() override;

	virtual void InitializeAttributes() {}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	// -------- Attribute Carrier Interface --------
	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool RemoveAttribute(FName InName) override;

	virtual bool ReplicateModifierDescObjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
protected:
	virtual bool OnAttributeAdded(TSharedPtr<FAttributeBase> InAttribute) override;
	virtual TSharedPtr<FAttributeBase> GetAttribute(FName InName) override;
	virtual TArray<TSharedPtr<FAttributeBase>> GetAllAttributes() override;

	virtual UObject* GetSubobjectCarrier() override { return this; }
	virtual UObject* ThisAsObject() override { return this; }
	virtual ENetRole GetNetRole() override { return GetOwnerRole(); }

	virtual void OnModifierInterfaceAdded(IAttributeModifierCarrierInterface* InModifierInterface) override;
	virtual void OnModifierObjectAdded(UObject* InModifierObject) override;
	virtual void OnModifierDescObjectAdded(UObject* InModifierObject, UAttributeModifierDescObject* InDescObject) override;
	virtual void OnModifierInterfaceRemoved(IAttributeModifierCarrierInterface* InModifierInterface) override;
	virtual void OnModifierObjectRemoved(UObject* InModifierObject) override;
	virtual TArray<IAttributeModifierCarrierInterface*> GetAllModifierObjects() const override;

	virtual void OnBooleanAttributeWrapperObjectCreated(UAttributeBooleanWrapperObject* NewWrapper) override;
	virtual void OnIntegerAttributeWrapperObjectCreated(UAttributeIntegerWrapperObject* NewWrapper) override;
	virtual void OnFloatAttributeWrapperObjectCreated(UAttributeFloatWrapperObject* NewWrapper) override;
	// -------- Attribute Carrier Interface --------

	UFUNCTION()
	void OnRep_BooleanWrapperList();

	UFUNCTION()
	void OnRep_IntegerWrapperList();

	UFUNCTION()
	void OnRep_FloatWrapperList();

protected:
	TMap<FName, TSharedPtr<FAttributeBase>> AttributeMap;

	TArray<IAttributeModifierCarrierInterface*> ModifierList;

	TMap<UObject*, TArray<UAttributeModifierDescObject*>> ModifierDescObjectMap;

	// Reference holder for modifiers
	UPROPERTY(Transient)
	TArray<UObject*> ModifierObjectList;

	// Replicated array for modifier description objects
	UPROPERTY(Transient, Replicated)
	TArray<UAttributeModifierDescObject*> ModifierDescList;

	// Replicated array for attribute wrapper objects
	UPROPERTY(Transient, ReplicatedUsing=OnRep_BooleanWrapperList)
	TArray<UAttributeBooleanWrapperObject*> BooleanWrapperList;

	UPROPERTY(Transient, ReplicatedUsing=OnRep_IntegerWrapperList)
	TArray<UAttributeIntegerWrapperObject*> IntegerWrapperList;

	UPROPERTY(Transient, ReplicatedUsing=OnRep_FloatWrapperList)
	TArray<UAttributeFloatWrapperObject*> FloatWrapperList;
};

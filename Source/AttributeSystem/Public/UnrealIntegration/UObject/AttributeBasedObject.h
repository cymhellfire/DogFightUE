#pragma once

#include "CoreMinimal.h"
#include "UnrealIntegration/Interface/AttributeCarrierInterface.h"
#include "AttributeBasedObject.generated.h"

UCLASS()
class ATTRIBUTESYSTEM_API UAttributeBasedObject : public UObject, public IAttributeCarrierInterface
{
	GENERATED_BODY()
public:
	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetReplicator(AActor* InActor);

	// -------- Attribute Carrier Interface --------'
	UFUNCTION(BlueprintCallable, Category="AttributeCarrier")
	virtual bool RemoveAttribute(FName InName) override;

	virtual bool ReplicateModifierDescObjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
protected:
	virtual bool OnAttributeAdded(TSharedPtr<FAttributeBase> InAttribute) override;
	virtual TSharedPtr<FAttributeBase> GetAttribute(FName InName) override;
	virtual TArray<TSharedPtr<FAttributeBase>> GetAllAttributes() override;

	virtual UObject* GetSubobjectCarrier() override { return this; }
	virtual UObject* ThisAsObject() override { return this; }
	virtual ENetRole GetNetRole() override
	{
		if (Replicator)
		{
			return Replicator->GetLocalRole();
		}

		// When this object is just created, the replicator is not set yet. We need a fallback to get usable net role.
		if (auto OuterActor = GetTypedOuter<AActor>())
		{
			return OuterActor->GetLocalRole();
		}
		return ROLE_None;
	}

	virtual void OnModifierInterfaceAdded(IAttributeModifierCarrierInterface* InModifierInterface) override;
	virtual void OnModifierObjectAdded(UObject* InModifierObject) override;
	virtual void OnModifierDescObjectAdded(UObject* InModifierObject, UAttributeModifierDescObject* InDescObject) override;
	virtual void OnModifierInterfaceRemoved(IAttributeModifierCarrierInterface* InModifierInterface) override;
	virtual void OnModifierObjectRemoved(UObject* InModifierObject) override;
	virtual TArray<IAttributeModifierCarrierInterface*> GetAllModifierObjects() const override;

	virtual void OnBooleanAttributeWrapperObjectCreated(UAttributeBooleanWrapperObject* NewWrapper) override;
	virtual void OnIntegerAttributeWrapperObjectCreated(UAttributeIntegerWrapperObject* NewWrapper) override;
	virtual void OnFloatAttributeWrapperObjectCreated(UAttributeFloatWrapperObject* NewWrapper) override;

	virtual TArray<UAttributeBooleanWrapperObject*> GetAllBooleanAttributeWrappers() const override { return BooleanWrapperList; }
	virtual TArray<UAttributeIntegerWrapperObject*> GetAllIntegerAttributeWrappers() const override { return IntegerWrapperList; }
	virtual TArray<UAttributeFloatWrapperObject*> GetAllFloatAttributeWrappers() const override { return FloatWrapperList; }

	UFUNCTION()
	virtual void OnRep_BooleanWrapperList() override;
	
	UFUNCTION()
	virtual void OnRep_IntegerWrapperList() override;
	
	UFUNCTION()
	virtual void OnRep_FloatWrapperList() override;
	// -------- Attribute Carrier Interface --------

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

	UPROPERTY(Transient, ReplicatedUsing=OnRep_BooleanWrapperList)
	TArray<UAttributeBooleanWrapperObject*> BooleanWrapperList;

	UPROPERTY(Transient, ReplicatedUsing=OnRep_IntegerWrapperList)
	TArray<UAttributeIntegerWrapperObject*> IntegerWrapperList;

	UPROPERTY(Transient, ReplicatedUsing=OnRep_FloatWrapperList)
	TArray<UAttributeFloatWrapperObject*> FloatWrapperList;

	UPROPERTY(Transient, Replicated)
	AActor* Replicator;
};

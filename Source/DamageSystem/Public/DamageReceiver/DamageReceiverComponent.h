#pragma once

#include "CoreMinimal.h"
#include "DamageType/ExtendedDamageInstance.h"
#include "UnrealIntegration/UObject/AttributeBasedComponent.h"
#include "DamageReceiverComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FDamageReceiverComponentTakeDamageSignature, UDamageReceiverComponent*,
	UExtendedDamageInstance*, const FExtendedDamageEvent&);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDamageReceiverComponentHealthChangeSignature, float, CurHealth, float, MaxHealth);

UCLASS()
class DAMAGESYSTEM_API UDamageReceiverComponent : public UAttributeBasedComponent
{
	GENERATED_BODY()
public:
	UDamageReceiverComponent(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeAttributes() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TakeDamage(UExtendedDamageInstance* DamageInstance, FExtendedDamageEvent InEvent);

	virtual void SetHealth(int32 InValue);

protected:
	virtual void Sync_OnIntegerWrapperAdded(UAttributeIntegerWrapperObject* InWrapper) override;

	void OnMaxHealthChanged(UAttributeIntegerWrapperObject* WrapperObject, int32 InValue);

	UFUNCTION()
	void OnRep_Health(int32 OldValue);

public:
	FDamageReceiverComponentTakeDamageSignature OnTakeDamage;

	FDamageReceiverComponentHealthChangeSignature OnHealthChanged;

protected:
	UPROPERTY(Transient, ReplicatedUsing=OnRep_Health)
	int32 Health;
};

#pragma once

#include "CoreMinimal.h"
#include "DamageType/ExtendedDamageInstance.h"
#include "UnrealIntegration/UObject/AttributeBasedComponent.h"
#include "DamageReceiverComponent.generated.h"

UCLASS()
class DAMAGESYSTEM_API UDamageReceiverComponent : public UAttributeBasedComponent
{
	GENERATED_BODY()
public:
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
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FDamageReceiverComponentTakeDamageSignature, UDamageReceiverComponent*,
		UExtendedDamageInstance*, const FExtendedDamageEvent&);
	FDamageReceiverComponentTakeDamageSignature OnTakeDamage;

protected:
	UPROPERTY(Transient, ReplicatedUsing=OnRep_Health)
	int32 Health;
};

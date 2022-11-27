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

protected:
	virtual void Sync_OnIntegerWrapperAdded(UAttributeIntegerWrapperObject* InWrapper) override;

	void OnMaxHealthChanged(UAttributeIntegerWrapperObject* WrapperObject, int32 InValue);

public:
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FDamageReceiverComponentTakeDamageSignature, UDamageReceiverComponent*,
		UExtendedDamageInstance*, const FExtendedDamageEvent&);
	FDamageReceiverComponentTakeDamageSignature OnTakeDamage;
};

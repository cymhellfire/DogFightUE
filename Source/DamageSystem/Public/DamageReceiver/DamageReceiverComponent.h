#pragma once

#include "CoreMinimal.h"
#include "UnrealIntegration/UObject/AttributeBasedComponent.h"
#include "AttributeSystem/Attribute/Attribute.h"
#include "DamageReceiverComponent.generated.h"

struct FExtendedDamageEvent;
class UExtendedDamageInstance;

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
	DECLARE_DYNAMIC_DELEGATE_ThreeParams(FDamageReceiverComponentTakeDamageSignature, UDamageReceiverComponent*, DamagedComponent,
		UExtendedDamageInstance*, DamageInstance, const FExtendedDamageEvent&, DamageEvent);
	FDamageReceiverComponentTakeDamageSignature OnTakeDamage;
};

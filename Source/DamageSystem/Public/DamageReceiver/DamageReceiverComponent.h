#pragma once

#include "CoreMinimal.h"
#include "UnrealIntegration/UObject/AttributeBasedComponent.h"
#include "AttributeSystem/Attribute/Attribute.h"
#include "UnrealIntegration/DataWrapper/AttributeWrapper.h"
#include "DamageReceiverComponent.generated.h"

class FAttributeBase;

UCLASS()
class DAMAGESYSTEM_API UDamageReceiverComponent : public UAttributeBasedComponent
{
	GENERATED_BODY()
public:
	virtual void InitializeAttributes() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	void OnMaxHealthChanged(TSharedPtr<FAttributeBase> InAttribute);

	UFUNCTION()
	void OnRep_MaxHealth(const FAttributeIntegerWrapper& OldValue);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DamageReceiverComponent", ReplicatedUsing=OnRep_MaxHealth)
	FAttributeIntegerWrapper MaxHealth;
};

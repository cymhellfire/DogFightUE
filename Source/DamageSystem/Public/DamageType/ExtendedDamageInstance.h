#pragma once

#include "ExtendedDamageInstance.generated.h"

class UExtendedDamageType;
class UDamageReceiverComponent;

USTRUCT(BlueprintType)
struct FExtendedDamageEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExtendedDamageEvent")
	float DamageValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExtendedDamageEvent")
	UDamageReceiverComponent* ReceiverComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExtendedDamageEvent")
	AActor* DamageCauser;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExtendedDamageEvent")
	AController* Instigator;
};

UCLASS(Blueprintable)
class DAMAGESYSTEM_API UExtendedDamageInstance : public UObject
{
	GENERATED_BODY()
public:
	virtual void PreApplyToComponent(FExtendedDamageEvent DamageEvent);

	virtual void ApplyToComponent(FExtendedDamageEvent DamageEvent);

	virtual void PostApplyToComponent(FExtendedDamageEvent DamageEvent);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="ExtendedDamageInstance", DisplayName="Pre Apply To Component")
	void K2_PreApplyToComponent(const FExtendedDamageEvent& DamageEvent);

	UFUNCTION(BlueprintImplementableEvent, Category="ExtendedDamageInstance", DisplayName="Post Apply To Component")
	void K2_PostApplyToComponent(const FExtendedDamageEvent& DamageEvent);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExtendedDamageInstance")
	TSubclassOf<UExtendedDamageType> DamageType;
};

#pragma once

#include "ExtendedDamageEvent.h"
#include "ExtendedDamageInstance.generated.h"

class UExtendedDamageType;
class UDamageReceiverComponent;

UCLASS(Blueprintable)
class DAMAGESYSTEM_API UExtendedDamageInstance : public UObject
{
	GENERATED_BODY()
public:
	virtual void PreApplyToComponent(FExtendedDamageEvent& DamageEvent);

	virtual void ApplyToComponent(const FExtendedDamageEvent& DamageEvent);

	virtual void PostApplyToComponent(FExtendedDamageEvent& DamageEvent);

	UFUNCTION()
	void SetDamageType(UClass* InType);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="ExtendedDamageInstance", DisplayName="Pre Apply To Component")
	void K2_PreApplyToComponent(FExtendedDamageEvent& DamageEvent);

	UFUNCTION(BlueprintImplementableEvent, Category="ExtendedDamageInstance", DisplayName="Post Apply To Component")
	void K2_PostApplyToComponent(FExtendedDamageEvent& DamageEvent);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ExtendedDamageInstance")
	TSubclassOf<UExtendedDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ExtendedDamageInstance")
	int32 Id;
};

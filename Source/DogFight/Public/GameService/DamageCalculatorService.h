#pragma once

#include "LuaGameService.h"
#include "DamageCalculatorService.generated.h"

class UDamageCalculatorBase;
class UExtendedDamageInstance;

UCLASS()
class DOGFIGHT_API UDamageCalculatorService : public ULuaGameService
{
	GENERATED_BODY()
public:
	UDamageCalculatorBase* CreateDamageCalculator(const FString& ScriptPath, UObject* Instigator);

	virtual FString GetModuleName_Implementation() const override
	{
		return TEXT("DogFight.Services.DamageCalculatorService");
	}

	UFUNCTION(BlueprintImplementableEvent, Category="DamageCalculatorService")
	UDamageCalculatorBase* LoadDamageCalculator(const FString& ScriptPath, UObject* Instigator);

protected:
	void InitializeDamageCalculator(UDamageCalculatorBase* InCalculator);

	UFUNCTION(BlueprintImplementableEvent, Category="DamageCalculatorService")
	UExtendedDamageInstance* LoadDamageInstance(const FString& ScriptPath, UObject* Instigator);

	UFUNCTION(BlueprintCallable)
	UClass* GetDamageCalculatorClass();

	UFUNCTION(BlueprintCallable)
	UClass* GetDamageInstanceClass();
};

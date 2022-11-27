#include "GameService/DamageCalculatorService.h"

#include "DamageCalculator/DamageCalculatorBase.h"
#include "DamageType/ExtendedDamageInstance.h"

UDamageCalculatorBase* UDamageCalculatorService::CreateDamageCalculator(const FString& ScriptPath, UObject* Instigator)
{
	// Create calculator first
	auto NewCalculator = LoadDamageCalculator(ScriptPath, Instigator);
	if (!NewCalculator)
	{
		return nullptr;
	}

	InitializeDamageCalculator(NewCalculator);
	return NewCalculator;
}

void UDamageCalculatorService::InitializeDamageCalculator(UDamageCalculatorBase* InCalculator)
{
	if (!InCalculator)
	{
		return;
	}

	auto DamageInstanceList = InCalculator->GetDamageInstanceList();
	for (auto InstanceName : DamageInstanceList)
	{
		if (InstanceName.IsEmpty())
			continue;

		auto NewInstance = LoadDamageInstance(InstanceName, InCalculator);
		if (NewInstance)
		{
			InCalculator->RegisterNewDamageInstance(FName(InstanceName), NewInstance);
		}
	}
}

UClass* UDamageCalculatorService::GetDamageCalculatorClass()
{
	return UDamageCalculatorBase::StaticClass();
}

UClass* UDamageCalculatorService::GetDamageInstanceClass()
{
	return UExtendedDamageInstance::StaticClass();
}

#include "DamageType/ExtendedDamageInstance.h"

#include "DamageReceiver/DamageReceiverComponent.h"
#include "DamageType/ExtendedDamageType.h"

void UExtendedDamageInstance::PreApplyToComponent(FExtendedDamageEvent& DamageEvent)
{
	// Invoke blueprint implementation
	K2_PreApplyToComponent(DamageEvent);
}

void UExtendedDamageInstance::ApplyToComponent(const FExtendedDamageEvent& DamageEvent)
{
	auto ReceiverComponent = DamageEvent.ReceiverComponent;
	if (ReceiverComponent)
	{
		ReceiverComponent->TakeDamage(this, DamageEvent);
	}
}

void UExtendedDamageInstance::PostApplyToComponent(FExtendedDamageEvent& DamageEvent)
{
	// Invoke blueprint implementation
	K2_PostApplyToComponent(DamageEvent);
}

void UExtendedDamageInstance::SetDamageType(UClass* InType)
{
	if (InType->IsChildOf(UExtendedDamageType::StaticClass()))
	{
		DamageType = InType;
	}
}

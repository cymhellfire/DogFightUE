#include "DamageReceiver/DamageReceiverComponent.h"
#include "DamageType/ExtendedDamageInstance.h"
#include "Net/UnrealNetwork.h"

void UDamageReceiverComponent::InitializeAttributes()
{
	Super::InitializeAttributes();

	// Max Health
	if (AddIntegerAttribute("MaxHealth", 100, "Health"))
	{
		auto NewAttribute = GetIntegerAttributeWrapperByName("MaxHealth");
		if (NewAttribute)
		{
			NewAttribute->OnValueChanged.AddUObject(this, &UDamageReceiverComponent::OnMaxHealthChanged);
		}
	}
}

void UDamageReceiverComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParam;
	SharedParam.bIsPushBased = true;

	//DOREPLIFETIME_WITH_PARAMS_FAST(UDamageReceiverComponent, MaxHealth, SharedParam);
}

void UDamageReceiverComponent::TakeDamage(UExtendedDamageInstance* DamageInstance, FExtendedDamageEvent InEvent)
{
	// Update health
	

	// Trigger delegate
	OnTakeDamage.Broadcast(this, DamageInstance, InEvent);
}

void UDamageReceiverComponent::Sync_OnIntegerWrapperAdded(UAttributeIntegerWrapperObject* InWrapper)
{
	Super::Sync_OnIntegerWrapperAdded(InWrapper);

	if (InWrapper->GetAttributeName() == "MaxHealth")
	{
		InWrapper->OnValueChanged.AddUObject(this, &UDamageReceiverComponent::OnMaxHealthChanged);
	}
}

void UDamageReceiverComponent::OnMaxHealthChanged(UAttributeIntegerWrapperObject* WrapperObject, int32 InValue)
{
	
}

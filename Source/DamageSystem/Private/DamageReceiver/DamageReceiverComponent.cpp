#include "DamageReceiver/DamageReceiverComponent.h"

#include "DamageSytemCommon.h"
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

			SetHealth(NewAttribute->GetValue());
		}
	}
}

void UDamageReceiverComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParam;
	SharedParam.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(UDamageReceiverComponent, Health, SharedParam);
}

void UDamageReceiverComponent::TakeDamage(UExtendedDamageInstance* DamageInstance, FExtendedDamageEvent InEvent)
{
	// Update health
	SetHealth(Health - InEvent.DamageValue);

	// Trigger delegate
	OnTakeDamage.Broadcast(this, DamageInstance, InEvent);
}

void UDamageReceiverComponent::SetHealth(int32 InValue)
{
	if (InValue == Health || GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	const int32 OldValue = Health;
	MARK_PROPERTY_DIRTY_FROM_NAME(UDamageReceiverComponent, Health, this);
	Health = InValue;

	OnRep_Health(OldValue);
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
	// Clamp current health
	SetHealth(FMath::Clamp(Health, Health, InValue));
}

void UDamageReceiverComponent::OnRep_Health(int32 OldValue)
{
	const FString NetRoleStr = TO_NET_ROLE_STR(GetOwnerRole());
	UE_LOG(LogDamageSystem, Log, TEXT("%s: [%s] Health %d -> %d"), *NetRoleStr, *GetName(), OldValue, Health);
}

#include "DamageReceiver/DamageReceiverComponent.h"

#include "DamageSytemCommon.h"
#include "DamageType/ExtendedDamageInstance.h"
#include "Net/UnrealNetwork.h"

UDamageReceiverComponent::UDamageReceiverComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Enable component replication
	SetIsReplicatedByDefault(true);

	// Enable auto add attribute
	bAddMissingAttributeForModifier = true;
}

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
	DOREPLIFETIME_WITH_PARAMS_FAST(UDamageReceiverComponent, MaxHealth, SharedParam);
	DOREPLIFETIME_WITH_PARAMS_FAST(UDamageReceiverComponent, bInvincible, SharedParam);
}

void UDamageReceiverComponent::TakeDamage(UExtendedDamageInstance* DamageInstance, const FExtendedDamageEvent& InEvent)
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

void UDamageReceiverComponent::SetInvincible(bool Value)
{
	if (Value == bInvincible || GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(UDamageReceiverComponent, bInvincible, this);
	bInvincible = Value;
}

void UDamageReceiverComponent::AddInvincibleCauser(UObject* Causer)
{
	if (IsValid(Causer))
	{
		InvincibleCauserList.AddUnique(Causer);

		if (InvincibleCauserList.Num() > 0)
		{
			SetInvincible(true);
		}
	}
}

void UDamageReceiverComponent::RemoveInvincibleCauser(UObject* Causer)
{
	if (IsValid(Causer))
	{
		InvincibleCauserList.Remove(Causer);

		if (InvincibleCauserList.Num() == 0)
		{
			SetInvincible(false);
		}
	}
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
	MARK_PROPERTY_DIRTY_FROM_NAME(UDamageReceiverComponent, MaxHealth, this);
	MaxHealth = InValue;

	// Clamp current health
	SetHealth(FMath::Clamp(Health, Health, MaxHealth));
}

void UDamageReceiverComponent::OnRep_Health(int32 OldValue)
{
	const FString NetRoleStr = TO_NET_ROLE_STR(GetOwnerRole());
	UE_LOG(LogDamageSystem, Log, TEXT("%s: [%s] Health %d -> %d"), *NetRoleStr, *GetName(), OldValue, Health);

	OnHealthChanged.Broadcast(Health, 100);

	// No health check
	if (Health <= 0.f)
	{
		OnNoHealth.Broadcast();
	}
}

void UDamageReceiverComponent::OnRep_MaxHealth(int32 OldValue)
{
	const FString NetRoleStr = TO_NET_ROLE_STR(GetOwnerRole());
	UE_LOG(LogDamageSystem, Log, TEXT("%s: [%s] MaxHealth %d -> %d"), *NetRoleStr, *GetName(), OldValue, MaxHealth);

	OnHealthChanged.Broadcast(Health, MaxHealth);
}

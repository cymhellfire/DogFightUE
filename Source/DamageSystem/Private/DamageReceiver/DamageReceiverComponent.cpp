#include "DamageReceiver/DamageReceiverComponent.h"

#include "Net/UnrealNetwork.h"

void UDamageReceiverComponent::InitializeAttributes()
{
	Super::InitializeAttributes();

	// Max Health
	if (AddIntegerAttribute("MaxHealth", 100, "Health"))
	{
		auto NewAttribute = GetBooleanAttributeWrapperByName("MaxHealth");
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

void UDamageReceiverComponent::OnMaxHealthChanged()
{
	
}

// void UDamageReceiverComponent::OnRep_MaxHealth(const FAttributeIntegerWrapper& OldValue)
// {
// 	UE_LOG(LogTemp, Log, TEXT("MaxHealth %d -> %d"), OldValue.Value, MaxHealth.Value);
// }

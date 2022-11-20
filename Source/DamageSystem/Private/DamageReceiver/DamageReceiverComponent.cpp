#include "DamageReceiver/DamageReceiverComponent.h"

#include "Net/UnrealNetwork.h"

void UDamageReceiverComponent::InitializeAttributes()
{
	Super::InitializeAttributes();

	// const FName MaxHealthName("MaxHealth");
	// FAttributeCreateArgument MaxHealthArgument
	// {
	// 	MaxHealthName,
	// 	ADT_Integer
	// };
	// MaxHealthArgument.InitIntegerValue = 100;
	// if (AddAttribute(MaxHealthArgument))
	// {
	// 	auto NewAttr = GetAttribute(MaxHealthName);
	// 	NewAttr->OnValueChanged.AddUObject(this, &UDamageReceiverComponent::OnMaxHealthChanged);
	// }
}

void UDamageReceiverComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParam;
	SharedParam.bIsPushBased = true;

	//DOREPLIFETIME_WITH_PARAMS_FAST(UDamageReceiverComponent, MaxHealth, SharedParam);
}

// void UDamageReceiverComponent::OnMaxHealthChanged(TSharedPtr<FAttributeBase> InAttribute)
// {
// 	if (auto ConvertedAttr = StaticCastSharedPtr<FAttributeInteger>(InAttribute))
// 	{
// 		MARK_PROPERTY_DIRTY_FROM_NAME(UDamageReceiverComponent, MaxHealth, this);
// 		MaxHealth.BaseValue = ConvertedAttr->GetValue();
// 	}
// }
//
// void UDamageReceiverComponent::OnRep_MaxHealth(const FAttributeIntegerWrapper& OldValue)
// {
// 	UE_LOG(LogTemp, Log, TEXT("MaxHealth %d -> %d"), OldValue.Value, MaxHealth.Value);
// }

#include "UnrealIntegration/UObject/AttributeModifierDescObject.h"
#include "Net/UnrealNetwork.h"

void UAttributeModifierDescObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeModifierDescObject, Source, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeModifierDescObject, Effect, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeModifierDescObject, Desc, SharedParams);
}

void UAttributeModifierDescObject::SetSourceString(FString InStr)
{
	if (InStr != Source)
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeModifierDescObject, Source, this);
		Source = InStr;
	}
}

void UAttributeModifierDescObject::SetEffectString(FString InStr)
{
	if (InStr != Effect)
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeModifierDescObject, Effect, this);
		Effect = InStr;
	}
}

void UAttributeModifierDescObject::SetDescString(FText InDesc)
{
	if (!InDesc.EqualTo(Desc))
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeModifierDescObject, Desc, this);
		Desc = InDesc;
	}
}

FString UAttributeModifierDescObject::ToString() const
{
	return FString::Printf(TEXT("Name - %s, Effect - %s"), *Source, *Effect);
}

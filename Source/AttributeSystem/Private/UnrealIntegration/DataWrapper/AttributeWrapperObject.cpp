#include "UnrealIntegration/DataWrapper/AttributeWrapperObject.h"
#include "Net/UnrealNetwork.h"

#define MAKE_SHARED_PARAMS(ParamName) \
	FDoRepLifetimeParams ParamName; \
	ParamName.bIsPushBased = true;

void UAttributeWrapperObjectBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	MAKE_SHARED_PARAMS(SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeWrapperObjectBase, AppliedModifierDesc, SharedParams);
}

void UAttributeWrapperObjectBase::AddDescObject(UAttributeModifierDescObject* InDescObject)
{
	if (!InDescObject)
		return;

	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeWrapperObjectBase, AppliedModifierDesc, this);
	AppliedModifierDesc.AddUnique(InDescObject);
}

void UAttributeWrapperObjectBase::RemoveDescObject(UAttributeModifierDescObject* InDescObject)
{
	if (!InDescObject)
		return;

	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeWrapperObjectBase, AppliedModifierDesc, this);
	AppliedModifierDesc.Remove(InDescObject);
}

void UAttributeBooleanWrapperObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	MAKE_SHARED_PARAMS(SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeBooleanWrapperObject, BaseValue, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeBooleanWrapperObject, Value, SharedParams);
}

void UAttributeBooleanWrapperObject::SetBaseValue(bool InValue)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeBooleanWrapperObject, BaseValue, this);
	BaseValue = InValue;
}

void UAttributeBooleanWrapperObject::SetValue(bool InValue)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeBooleanWrapperObject, Value, this);
	Value = InValue;
}

void UAttributeBooleanWrapperObject::OnRep_BaseValue()
{
	OnBaseValueChanged.Broadcast();
}

void UAttributeBooleanWrapperObject::OnRep_Value()
{
	OnValueChanged.Broadcast();
}

void UAttributeIntegerWrapperObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	MAKE_SHARED_PARAMS(SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeIntegerWrapperObject, BaseValue, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeIntegerWrapperObject, Value, SharedParams);
}

void UAttributeIntegerWrapperObject::SetBaseValue(int32 InValue)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeIntegerWrapperObject, BaseValue, this);
	BaseValue = InValue;
}

void UAttributeIntegerWrapperObject::SetValue(int32 InValue)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeIntegerWrapperObject, Value, this);
	Value = InValue;
}

void UAttributeIntegerWrapperObject::OnRep_BaseValue()
{
	OnBaseValueChanged.Broadcast();
}

void UAttributeIntegerWrapperObject::OnRep_Value()
{
	OnValueChanged.Broadcast();
}

void UAttributeFloatWrapperObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	MAKE_SHARED_PARAMS(SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeFloatWrapperObject, BaseValue, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeFloatWrapperObject, Value, SharedParams);
}

void UAttributeFloatWrapperObject::SetBaseValue(float InValue)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeFloatWrapperObject, BaseValue, this);
	BaseValue = InValue;
}

void UAttributeFloatWrapperObject::SetValue(float InValue)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeFloatWrapperObject, Value, this);
	Value = InValue;
}

void UAttributeFloatWrapperObject::OnRep_BaseValue()
{
	OnBaseValueChanged.Broadcast();
}

void UAttributeFloatWrapperObject::OnRep_Value()
{
	OnValueChanged.Broadcast();
}

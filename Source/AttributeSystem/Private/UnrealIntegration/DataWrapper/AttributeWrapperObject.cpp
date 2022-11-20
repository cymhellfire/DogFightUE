#include "UnrealIntegration/DataWrapper/AttributeWrapperObject.h"

#include "AttributeSystem.h"
#include "Net/UnrealNetwork.h"
#include "UnrealIntegration/UObject/AttributeModifierDescObject.h"

#define MAKE_SHARED_PARAMS(ParamName) \
	FDoRepLifetimeParams ParamName; \
	ParamName.bIsPushBased = true;

void UAttributeWrapperObjectBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	MAKE_SHARED_PARAMS(SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeWrapperObjectBase, AttributeName, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeWrapperObjectBase, AppliedModifierDesc, SharedParams);
}

void UAttributeWrapperObjectBase::AddDescObject(UAttributeModifierDescObject* InDescObject)
{
	if (!InDescObject)
		return;

	const TArray<UAttributeModifierDescObject*> OldValue = TArray(AppliedModifierDesc);
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeWrapperObjectBase, AppliedModifierDesc, this);
	AppliedModifierDesc.AddUnique(InDescObject);

	OnRep_AppliedModifierDesc(OldValue);
}

void UAttributeWrapperObjectBase::RemoveDescObject(UAttributeModifierDescObject* InDescObject)
{
	if (!InDescObject)
		return;

	const TArray<UAttributeModifierDescObject*> OldValue = TArray(AppliedModifierDesc);
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeWrapperObjectBase, AppliedModifierDesc, this);
	AppliedModifierDesc.Remove(InDescObject);

	OnRep_AppliedModifierDesc(OldValue);
}

void UAttributeWrapperObjectBase::SetAttributeName(FName InName)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeWrapperObjectBase, AttributeName, this);
	AttributeName = InName;
}

void UAttributeWrapperObjectBase::OnRep_AppliedModifierDesc(const TArray<UAttributeModifierDescObject*>& OldList)
{
	TArray<UAttributeModifierDescObject*> RemovedObjects = TArray(OldList);
	for (auto Desc : AppliedModifierDesc)
	{
		if (RemovedObjects.Contains(Desc))
		{
			RemovedObjects.Remove(Desc);
		}
		else
		{
			UE_LOG(LogAttributeSystem, Log, TEXT("[%s] Modifier Added: Name - %s Effect - %s"), *GetAttributeName().ToString(),
				*Desc->GetSourceString(), *Desc->GetEffectString());
		}
	}

	for (auto Desc : RemovedObjects)
	{
		UE_LOG(LogAttributeSystem, Log, TEXT("[%s] Modifier Removed: Name - %s Effect - %s"), *GetAttributeName().ToString(),
			*Desc->GetSourceString(), *Desc->GetEffectString());
	}
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
	const bool OldValue = BaseValue;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeBooleanWrapperObject, BaseValue, this);
	BaseValue = InValue;

	OnRep_BaseValue(OldValue);
}

void UAttributeBooleanWrapperObject::SetValue(bool InValue)
{
	const bool OldValue = Value;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeBooleanWrapperObject, Value, this);
	Value = InValue;

	OnRep_Value(OldValue);
}

void UAttributeBooleanWrapperObject::OnRep_BaseValue(bool OldValue)
{
	UE_LOG(LogAttributeSystem, Log, TEXT("[BooleanWrapperObject] Attribute %s BaseValue: %s -> %s"), *GetAttributeName().ToString(),
		BOOL_TO_STR(OldValue), BOOL_TO_STR(BaseValue));

	OnBaseValueChanged.Broadcast();
}

void UAttributeBooleanWrapperObject::OnRep_Value(bool OldValue)
{
	UE_LOG(LogAttributeSystem, Log, TEXT("[BooleanWrapperObject] Attribute %s Value: %s -> %s"), *GetAttributeName().ToString(),
		BOOL_TO_STR(OldValue), BOOL_TO_STR(Value));

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
	const int32 OldValue = BaseValue;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeIntegerWrapperObject, BaseValue, this);
	BaseValue = InValue;

	OnRep_BaseValue(OldValue);
}

void UAttributeIntegerWrapperObject::SetValue(int32 InValue)
{
	const int32 OldValue = Value;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeIntegerWrapperObject, Value, this);
	Value = InValue;

	OnRep_Value(OldValue);
}

void UAttributeIntegerWrapperObject::OnRep_BaseValue(int32 OldValue)
{
	UE_LOG(LogAttributeSystem, Log, TEXT("[IntegerWrapperObject] Attribute %s BaseValue: %d -> %d"), *GetAttributeName().ToString(),
		OldValue, BaseValue);

	OnBaseValueChanged.Broadcast();
}

void UAttributeIntegerWrapperObject::OnRep_Value(int32 OldValue)
{
	UE_LOG(LogAttributeSystem, Log, TEXT("[IntegerWrapperObject] Attribute %s Value: %d -> %d"), *GetAttributeName().ToString(),
		OldValue, Value);

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
	const float OldValue = BaseValue;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeFloatWrapperObject, BaseValue, this);
	BaseValue = InValue;

	OnRep_BaseValue(OldValue);
}

void UAttributeFloatWrapperObject::SetValue(float InValue)
{
	const float OldValue = Value;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeFloatWrapperObject, Value, this);
	Value = InValue;

	OnRep_Value(OldValue);
}

void UAttributeFloatWrapperObject::OnRep_BaseValue(float OldValue)
{
	UE_LOG(LogAttributeSystem, Log, TEXT("[FloatWrapperObject] Attribute %s BaseValue: %.3f -> %.3f"), *GetAttributeName().ToString(),
		OldValue, BaseValue);

	OnBaseValueChanged.Broadcast();
}

void UAttributeFloatWrapperObject::OnRep_Value(float OldValue)
{
	UE_LOG(LogAttributeSystem, Log, TEXT("[FloatWrapperObject] Attribute %s Value: %.3f -> %.3f"), *GetAttributeName().ToString(),
		OldValue, Value);

	OnValueChanged.Broadcast();
}

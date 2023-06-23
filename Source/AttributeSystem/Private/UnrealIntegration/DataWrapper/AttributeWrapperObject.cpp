#include "UnrealIntegration/DataWrapper/AttributeWrapperObject.h"

#include "AttributeSystem.h"
#include "AttributeSystem/Attribute/Attribute.h"
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
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeWrapperObjectBase, DisplayName, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeWrapperObjectBase, AppliedModifierDesc, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeWrapperObjectBase, AttributeFlag, SharedParams);
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

void UAttributeWrapperObjectBase::SetDisplayName(const FText& InName)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeWrapperObjectBase, DisplayName, this);
	DisplayName = InName;
}

void UAttributeWrapperObjectBase::OnRep_AppliedModifierDesc(const TArray<UAttributeModifierDescObject*>& OldList)
{
#if ATTR_DETAIL_LOG
	const FString ObjName = GetOuter()->GetName();
	const FString NetRoleStr = (GetNetRole() == ROLE_Authority ? TEXT("Host") : TEXT("Client"));
	TArray<UAttributeModifierDescObject*> RemovedObjects = TArray(OldList);
	for (auto Desc : AppliedModifierDesc)
	{
		if (RemovedObjects.Contains(Desc))
		{
			RemovedObjects.Remove(Desc);
		}
		else
		{
			UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] Modifier added to %s: %s"), *NetRoleStr, *ObjName,
				*GetAttributeName().ToString(),	*Desc->ToString());
		}
	}

	for (auto Desc : RemovedObjects)
	{
		UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] Modifier removed from %s: %s"), *NetRoleStr, *ObjName,
				*GetAttributeName().ToString(),	*Desc->ToString());
	}
#endif
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
#if ATTR_DETAIL_LOG
	const FString ObjName = GetOuter()->GetName();
	const FString NetRoleStr = (GetNetRole() == ROLE_Authority ? TEXT("Host") : TEXT("Client"));
	UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] Attribute %s BaseValue: %s -> %s"), *NetRoleStr, *ObjName,
		*GetAttributeName().ToString(),	BOOL_TO_STR(OldValue), BOOL_TO_STR(BaseValue));
#endif

	OnBaseValueChanged.Broadcast(this, BaseValue);
}

void UAttributeBooleanWrapperObject::OnRep_Value(bool OldValue)
{
#if ATTR_DETAIL_LOG
	const FString ObjName = GetOuter()->GetName();
	const FString NetRoleStr = (GetNetRole() == ROLE_Authority ? TEXT("Host") : TEXT("Client"));
	UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] Attribute %s Value: %s -> %s"), *NetRoleStr, *ObjName,
		*GetAttributeName().ToString(), BOOL_TO_STR(OldValue), BOOL_TO_STR(Value));
#endif

	OnValueChanged.Broadcast(this, Value);
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
#if ATTR_DETAIL_LOG
	const FString ObjName = GetOuter()->GetName();
	const FString NetRoleStr = (GetNetRole() == ROLE_Authority ? TEXT("Host") : TEXT("Client"));
	UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] Attribute %s BaseValue: %d -> %d"), *NetRoleStr, *ObjName,
		*GetAttributeName().ToString(),	OldValue, BaseValue);
#endif

	OnBaseValueChanged.Broadcast(this, BaseValue);
}

void UAttributeIntegerWrapperObject::OnRep_Value(int32 OldValue)
{
#if ATTR_DETAIL_LOG
	const FString ObjName = GetOuter()->GetName();
	const FString NetRoleStr = (GetNetRole() == ROLE_Authority ? TEXT("Host") : TEXT("Client"));
	UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] Attribute %s Value: %d -> %d"), *NetRoleStr, *ObjName,
		*GetAttributeName().ToString(), OldValue, Value);
#endif

	OnValueChanged.Broadcast(this, Value);
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
#if ATTR_DETAIL_LOG
	const FString ObjName = GetOuter()->GetName();
	const FString NetRoleStr = (GetNetRole() == ROLE_Authority ? TEXT("Host") : TEXT("Client"));
	UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] Attribute %s BaseValue: %.3f -> %.3f"), *NetRoleStr, *ObjName,
		*GetAttributeName().ToString(),	OldValue, BaseValue);
#endif

	OnBaseValueChanged.Broadcast(this, BaseValue);
}

void UAttributeFloatWrapperObject::OnRep_Value(float OldValue)
{
#if ATTR_DETAIL_LOG
	const FString ObjName = GetOuter()->GetName();
	const FString NetRoleStr = (GetNetRole() == ROLE_Authority ? TEXT("Host") : TEXT("Client"));
	UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] Attribute %s Value: %.3f -> %.3f"), *NetRoleStr, *ObjName,
		*GetAttributeName().ToString(),	OldValue, Value);
#endif

	OnValueChanged.Broadcast(this, Value);
}

void FAttributeWrapperObjectHelper::InitializeCommonVariables(UAttributeWrapperObjectBase* InWrapper,
	TSharedPtr<FAttributeBase> InAttribute)
{
	if (!IsValid(InWrapper) || !InAttribute.IsValid())
	{
		return;
	}

	InWrapper->SetRawAttribute(InAttribute);
	InWrapper->SetAttributeName(InAttribute->GetName());
	InWrapper->SetDisplayName(InAttribute->GetDisplayName());
	InWrapper->SetAttributeFlag(InAttribute->AttributeFlag);

	InAttribute->SetWrapperObject(InWrapper);
}

UAttributeBooleanWrapperObject* FAttributeWrapperObjectHelper::CreateWrapperObjectForBooleanAttribute(UObject* Instigator,
	TSharedPtr<FAttributeBoolean> InAttribute, const TFunction<void(TSharedPtr<FAttributeBase>)>& InCallback)
{
	const FName AttributeName = InAttribute->GetName();
	UAttributeBooleanWrapperObject* NewWrapper = NewObject<UAttributeBooleanWrapperObject>(Instigator, NAME_None, RF_Transient);
	InitializeCommonVariables(NewWrapper, InAttribute);
	NewWrapper->SetBaseValue(InAttribute->GetRawValue());
	NewWrapper->SetValue(InAttribute->GetValue());
	InAttribute->OnValueChanged.AddLambda(InCallback);

	return NewWrapper;
}

UAttributeIntegerWrapperObject* FAttributeWrapperObjectHelper::CreateWrapperObjectForIntegerAttribute(UObject* Instigator,
	TSharedPtr<FAttributeInteger> InAttribute, const TFunction<void(TSharedPtr<FAttributeBase>)>& InCallback)
{
	const FName AttributeName = InAttribute->GetName();
	UAttributeIntegerWrapperObject* NewWrapper = NewObject<UAttributeIntegerWrapperObject>(Instigator, NAME_None, RF_Transient);
	InitializeCommonVariables(NewWrapper, InAttribute);
	NewWrapper->SetBaseValue(InAttribute->GetRawValue());
	NewWrapper->SetValue(InAttribute->GetValue());
	InAttribute->OnValueChanged.AddLambda(InCallback);

	return NewWrapper;
}

UAttributeFloatWrapperObject* FAttributeWrapperObjectHelper::CreateWrapperObjectForFloatAttribute(UObject* Instigator,
	TSharedPtr<FAttributeFloat> InAttribute, const TFunction<void(TSharedPtr<FAttributeBase>)>& InCallback)
{
	const FName AttributeName = InAttribute->GetName();
	UAttributeFloatWrapperObject* NewWrapper = NewObject<UAttributeFloatWrapperObject>(Instigator, NAME_None, RF_Transient);
	InitializeCommonVariables(NewWrapper, InAttribute);
	NewWrapper->SetBaseValue(InAttribute->GetRawValue());
	NewWrapper->SetValue(InAttribute->GetValue());
	InAttribute->OnValueChanged.AddLambda(InCallback);

	return NewWrapper;
}

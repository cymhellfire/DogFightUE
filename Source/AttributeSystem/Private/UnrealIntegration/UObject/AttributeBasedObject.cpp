#include "UnrealIntegration/UObject/AttributeBasedObject.h"

#include "AttributeSystem.h"
#include "UnrealIntegration/UObject/AttributeModifierDescObject.h"

#include "AttributeSystem/Attribute/AttributeBase.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

void UAttributeBasedObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeBasedObject, ModifierDescList, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeBasedObject, BooleanWrapperList, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeBasedObject, IntegerWrapperList, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeBasedObject, FloatWrapperList, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeBasedObject, Replicator, SharedParams);
}

void UAttributeBasedObject::SetReplicator(AActor* InActor)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeBasedObject, Replicator, this);
	Replicator = InActor;
}

bool UAttributeBasedObject::AddAttribute(const FAttributeCreateArgument& InArgument)
{
	return IAttributeCarrierInterface::AddAttribute(InArgument);
}

bool UAttributeBasedObject::GetAttributeBoolValue(FName InName, bool& OutValue)
{
	return IAttributeCarrierInterface::GetAttributeBoolValue(InName, OutValue);
}

bool UAttributeBasedObject::GetAttributeIntegerValue(FName InName, int32& OutValue)
{
	return IAttributeCarrierInterface::GetAttributeIntegerValue(InName, OutValue);
}

bool UAttributeBasedObject::GetAttributeFloatValue(FName InName, float& OutValue)
{
	return IAttributeCarrierInterface::GetAttributeFloatValue(InName, OutValue);
}

bool UAttributeBasedObject::RemoveAttribute(FName InName)
{
	return AttributeMap.Remove(InName) == 1;
}

bool UAttributeBasedObject::AddModifierObject(TScriptInterface<IAttributeModifierCarrierInterface> InModifierObject)
{
	return IAttributeCarrierInterface::AddModifierObject(InModifierObject);
}

bool UAttributeBasedObject::RemoveModifierObject(TScriptInterface<IAttributeModifierCarrierInterface> InModifierObject)
{
	return IAttributeCarrierInterface::RemoveModifierObject(InModifierObject);
}

bool UAttributeBasedObject::ReplicateModifierDescObjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = false;

	// Replicate array content
	for (auto DescObject : ModifierDescList)
	{
		bWroteSomething |= Channel->ReplicateSubobject(DescObject, *Bunch, *RepFlags);
	}

	// Replicate wrapper list content
	for (auto Wrapper : BooleanWrapperList)
	{
		bWroteSomething |= Channel->ReplicateSubobject(Wrapper, *Bunch, *RepFlags);
	}
	for (auto Wrapper : IntegerWrapperList)
	{
		bWroteSomething |= Channel->ReplicateSubobject(Wrapper, *Bunch, *RepFlags);
	}
	for (auto Wrapper : FloatWrapperList)
	{
		bWroteSomething |= Channel->ReplicateSubobject(Wrapper, *Bunch, *RepFlags);
	}

	return bWroteSomething;
}

bool UAttributeBasedObject::OnAttributeAdded(TSharedPtr<FAttributeBase> InAttribute)
{
	if (!InAttribute.IsValid() || AttributeMap.Contains(InAttribute->GetName()))
	{
		return false;
	}

	AttributeMap.Add(InAttribute->GetName(), InAttribute);
	return true;
}

TSharedPtr<FAttributeBase> UAttributeBasedObject::GetAttribute(FName InName)
{
	auto AttrPtr = AttributeMap.Find(InName);
	if (AttrPtr)
	{
		return *AttrPtr;
	}

	return nullptr;
}

TArray<TSharedPtr<FAttributeBase>> UAttributeBasedObject::GetAllAttributes()
{
	TArray<TSharedPtr<FAttributeBase>> Result;
	AttributeMap.GenerateValueArray(Result);
	return Result;
}

void UAttributeBasedObject::OnModifierInterfaceAdded(IAttributeModifierCarrierInterface* InModifierInterface)
{
	if (InModifierInterface == nullptr)
	{
		return;
	}

	ModifierList.AddUnique(InModifierInterface);
}

void UAttributeBasedObject::OnModifierObjectAdded(UObject* InModifierObject)
{
	if (InModifierObject == nullptr)
	{
		return;
	}

	ModifierObjectList.AddUnique(InModifierObject);
}

void UAttributeBasedObject::OnModifierDescObjectAdded(UObject* InModifierObject, UAttributeModifierDescObject* InDescObject)
{
	if (InModifierObject == nullptr || InDescObject == nullptr)
	{
		return;
	}

	// Record new desc object in map
	TArray<UAttributeModifierDescObject*> TargetList = ModifierDescObjectMap.FindOrAdd(InModifierObject);
	TargetList.Add(InDescObject);

	// Record new desc object into replicated array
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeBasedObject, ModifierDescList, this);
	ModifierDescList.Add(InDescObject);
}

void UAttributeBasedObject::OnModifierInterfaceRemoved(IAttributeModifierCarrierInterface* InModifierInterface)
{
	if (InModifierInterface == nullptr)
	{
		return;
	}

	ModifierList.Remove(InModifierInterface);
}

void UAttributeBasedObject::OnModifierObjectRemoved(UObject* InModifierObject)
{
	if (InModifierObject == nullptr)
	{
		return;
	}

	ModifierObjectList.Remove(InModifierObject);
}

TArray<IAttributeModifierCarrierInterface*> UAttributeBasedObject::GetAllModifierObjects() const
{
	return ModifierList;
}

void UAttributeBasedObject::OnBooleanAttributeWrapperObjectCreated(UAttributeBooleanWrapperObject* NewWrapper)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeBasedObject, BooleanWrapperList, this);
	BooleanWrapperList.Add(NewWrapper);

	OnRep_BooleanWrapperList();
}

void UAttributeBasedObject::OnIntegerAttributeWrapperObjectCreated(UAttributeIntegerWrapperObject* NewWrapper)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeBasedObject, IntegerWrapperList, this);
	IntegerWrapperList.Add(NewWrapper);

	OnRep_IntegerWrapperList();
}

void UAttributeBasedObject::OnFloatAttributeWrapperObjectCreated(UAttributeFloatWrapperObject* NewWrapper)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeBasedObject, FloatWrapperList, this);
	FloatWrapperList.Add(NewWrapper);

	OnRep_FloatWrapperList();
}

void UAttributeBasedObject::OnRep_BooleanWrapperList()
{
#if ATTR_DETAIL_LOG
	TArray<UAttributeBooleanWrapperObject*> NewAddedWrappers;
#endif
	for (auto Wrapper : BooleanWrapperList)
	{
		if (!Wrapper)
			continue;

		const FName AttributeName = Wrapper->GetAttributeName();
		// Add new synced wrapper to map
		if (!BooleanWrapperMap.Contains(AttributeName))
		{
			BooleanWrapperMap.Add(AttributeName, Wrapper);
#if ATTR_DETAIL_LOG
			NewAddedWrappers.Add(Wrapper);
#endif
		}
	}

#if ATTR_DETAIL_LOG
	if (NewAddedWrappers.Num() > 0)
	{
		const FString ObjName = GetName();
		const FString NetRoleStr = (GetNetRole() == ROLE_Authority ? TEXT("Host") : TEXT("Client"));
		for (auto Wrapper : NewAddedWrappers)
		{
			UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] New attribute %s added."), *NetRoleStr, *ObjName,
				*Wrapper->GetAttributeName().ToString());
			UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] %s"), *NetRoleStr, *ObjName, *Wrapper->ToString());
		}
	}

	TArray<FName> InvalidKeys;
	ValidateWrapperObjectMap(ADT_Boolean, &InvalidKeys);

	if (InvalidKeys.Num() > 0)
	{
		const FString ObjName = GetName();
		const FString NetRoleStr = (GetNetRole() == ROLE_Authority ? TEXT("Host") : TEXT("Client"));
		for (auto Key : InvalidKeys)
		{
			UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] Attribute %s removed."), *NetRoleStr, *ObjName, *Key.ToString());
		}
	}
#else
	ValidateWrapperObjectMap(ADT_Boolean);
#endif
}

void UAttributeBasedObject::OnRep_IntegerWrapperList()
{
#if ATTR_DETAIL_LOG
	TArray<UAttributeIntegerWrapperObject*> NewAddedWrappers;
#endif
	for (auto Wrapper : IntegerWrapperList)
	{
		if (!Wrapper)
			continue;

		const FName AttributeName = Wrapper->GetAttributeName();
		// Add new synced wrapper to map
		if (!IntegerWrapperMap.Contains(AttributeName))
		{
			IntegerWrapperMap.Add(AttributeName, Wrapper);
#if ATTR_DETAIL_LOG
			NewAddedWrappers.Add(Wrapper);
#endif
		}
	}

#if ATTR_DETAIL_LOG
	if (NewAddedWrappers.Num() > 0)
	{
		const FString ObjName = GetName();
		const FString NetRoleStr = (GetNetRole() == ROLE_Authority ? TEXT("Host") : TEXT("Client"));
		for (auto Wrapper : NewAddedWrappers)
		{
			UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] New attribute %s added."), *NetRoleStr, *ObjName,
				*Wrapper->GetAttributeName().ToString());
			UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] %s"), *NetRoleStr, *ObjName, *Wrapper->ToString());
		}
	}

	TArray<FName> InvalidKeys;
	ValidateWrapperObjectMap(ADT_Integer, &InvalidKeys);

	if (InvalidKeys.Num() > 0)
	{
		const FString ObjName = GetName();
		const FString NetRoleStr = (GetNetRole() == ROLE_Authority ? TEXT("Host") : TEXT("Client"));
		for (auto Key : InvalidKeys)
		{
			UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] Attribute %s removed."), *NetRoleStr, *ObjName, *Key.ToString());
		}
	}
#else
	ValidateWrapperObjectMap(ADT_Integer);
#endif
}

void UAttributeBasedObject::OnRep_FloatWrapperList()
{
#if ATTR_DETAIL_LOG
	TArray<UAttributeFloatWrapperObject*> NewAddedWrappers;
#endif
	for (auto Wrapper : FloatWrapperList)
	{
		if (!Wrapper)
			return;

		const FName AttributeName = Wrapper->GetAttributeName();
		// Add new synced wrapper to map
		if (!FloatWrapperMap.Contains(AttributeName))
		{
			FloatWrapperMap.Add(AttributeName, Wrapper);
#if ATTR_DETAIL_LOG
			NewAddedWrappers.Add(Wrapper);
#endif
		}
	}

#if ATTR_DETAIL_LOG
	if (NewAddedWrappers.Num() > 0)
	{
		const FString ObjName = GetName();
		const FString NetRoleStr = (GetNetRole() == ROLE_Authority ? TEXT("Host") : TEXT("Client"));
		for (auto Wrapper : NewAddedWrappers)
		{
			UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] New attribute %s added."), *NetRoleStr, *ObjName,
				*Wrapper->GetAttributeName().ToString());
			UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] %s"), *NetRoleStr, *ObjName, *Wrapper->ToString());
		}
	}

	TArray<FName> InvalidKeys;
	ValidateWrapperObjectMap(ADT_Float, &InvalidKeys);

	if (InvalidKeys.Num() > 0)
	{
		const FString ObjName = GetName();
		const FString NetRoleStr = (GetNetRole() == ROLE_Authority ? TEXT("Host") : TEXT("Client"));
		for (auto Key : InvalidKeys)
		{
			UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] Attribute %s removed."), *NetRoleStr, *ObjName, *Key.ToString());
		}
	}
#else
	ValidateWrapperObjectMap(ADT_Float);
#endif
}

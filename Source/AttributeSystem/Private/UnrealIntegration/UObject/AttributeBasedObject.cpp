#include "UnrealIntegration/UObject/AttributeBasedObject.h"
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

#include "UnrealIntegration/UObject/AttributeBasedComponent.h"

#include "AttributeSystem.h"
#include "AttributeSystem/Attribute/AttributeBase.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "UnrealIntegration/UObject/AttributeModifierDescObject.h"

UAttributeBasedComponent::UAttributeBasedComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UAttributeBasedComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Invoke attribute initializer on server/host side
	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		InitializeAttributes();
	}
}

void UAttributeBasedComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeBasedComponent, ModifierDescList, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeBasedComponent, BooleanWrapperList, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeBasedComponent, IntegerWrapperList, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeBasedComponent, FloatWrapperList, SharedParams);
}

bool UAttributeBasedComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	// Replicate description objects
	bWroteSomething |= ReplicateModifierDescObjects(Channel, Bunch, RepFlags);

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

bool UAttributeBasedComponent::AddAttribute(const FAttributeCreateArgument& InArgument)
{
	return IAttributeCarrierInterface::AddAttribute(InArgument);
}

bool UAttributeBasedComponent::GetAttributeBoolValue(FName InName, bool& OutValue)
{
	return IAttributeCarrierInterface::GetAttributeBoolValue(InName, OutValue);
}

bool UAttributeBasedComponent::GetAttributeIntegerValue(FName InName, int32& OutValue)
{
	return IAttributeCarrierInterface::GetAttributeIntegerValue(InName, OutValue);
}

bool UAttributeBasedComponent::GetAttributeFloatValue(FName InName, float& OutValue)
{
	return IAttributeCarrierInterface::GetAttributeFloatValue(InName, OutValue);
}

bool UAttributeBasedComponent::RemoveAttribute(FName InName)
{
	return AttributeMap.Remove(InName) == 1;
}

bool UAttributeBasedComponent::AddModifierObject(TScriptInterface<IAttributeModifierCarrierInterface> InModifierObject)
{
	return IAttributeCarrierInterface::AddModifierObject(InModifierObject);
}

bool UAttributeBasedComponent::RemoveModifierObject(TScriptInterface<IAttributeModifierCarrierInterface> InModifierObject)
{
	return IAttributeCarrierInterface::RemoveModifierObject(InModifierObject);
}

bool UAttributeBasedComponent::ReplicateModifierDescObjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = false;

	// Replicate array content
	for (auto DescObject : ModifierDescList)
	{
		bWroteSomething |= Channel->ReplicateSubobject(DescObject, *Bunch, *RepFlags);
	}

	return bWroteSomething;
}

bool UAttributeBasedComponent::OnAttributeAdded(TSharedPtr<FAttributeBase> InAttribute)
{
	if (!InAttribute.IsValid() || AttributeMap.Contains(InAttribute->GetName()))
	{
		return false;
	}

	// Record new attribute into map
	AttributeMap.Add(InAttribute->GetName(), InAttribute);

	return true;
}

TSharedPtr<FAttributeBase> UAttributeBasedComponent::GetAttribute(FName InName)
{
	auto AttrPtr = AttributeMap.Find(InName);
	if (AttrPtr)
	{
		return *AttrPtr;
	}

	return nullptr;
}

TArray<TSharedPtr<FAttributeBase>> UAttributeBasedComponent::GetAllAttributes()
{
	TArray<TSharedPtr<FAttributeBase>> Result;
	AttributeMap.GenerateValueArray(Result);
	return Result;
}

void UAttributeBasedComponent::OnModifierInterfaceAdded(IAttributeModifierCarrierInterface* InModifierInterface)
{
	if (InModifierInterface == nullptr)
	{
		return;
	}

	ModifierList.AddUnique(InModifierInterface);
}

void UAttributeBasedComponent::OnModifierObjectAdded(UObject* InModifierObject)
{
	if (InModifierObject == nullptr)
	{
		return;
	}

	ModifierObjectList.AddUnique(InModifierObject);
}

void UAttributeBasedComponent::OnModifierDescObjectAdded(UObject* InModifierObject, UAttributeModifierDescObject* InDescObject)
{
	if (InModifierObject == nullptr || InDescObject == nullptr)
	{
		return;
	}

	// Record new desc object in map
	TArray<UAttributeModifierDescObject*> TargetList = ModifierDescObjectMap.FindOrAdd(InModifierObject);
	TargetList.Add(InDescObject);

	// Record new desc object into replicated array
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeBasedComponent, ModifierDescList, this);
	ModifierDescList.Add(InDescObject);
}

void UAttributeBasedComponent::OnModifierInterfaceRemoved(IAttributeModifierCarrierInterface* InModifierInterface)
{
	if (InModifierInterface == nullptr)
	{
		return;
	}

	ModifierList.Remove(InModifierInterface);
}

void UAttributeBasedComponent::OnModifierObjectRemoved(UObject* InModifierObject)
{
	if (InModifierObject == nullptr)
	{
		return;
	}

	ModifierObjectList.Remove(InModifierObject);
}

TArray<IAttributeModifierCarrierInterface*> UAttributeBasedComponent::GetAllModifierObjects() const
{
	return ModifierList;
}

void UAttributeBasedComponent::OnBooleanAttributeWrapperObjectCreated(UAttributeBooleanWrapperObject* NewWrapper)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeBasedComponent, BooleanWrapperList, this);
	BooleanWrapperList.Add(NewWrapper);

	OnRep_BooleanWrapperList();
}

void UAttributeBasedComponent::OnIntegerAttributeWrapperObjectCreated(UAttributeIntegerWrapperObject* NewWrapper)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeBasedComponent, IntegerWrapperList, this);
	IntegerWrapperList.Add(NewWrapper);

	OnRep_IntegerWrapperList();
}

void UAttributeBasedComponent::OnFloatAttributeWrapperObjectCreated(UAttributeFloatWrapperObject* NewWrapper)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttributeBasedComponent, FloatWrapperList, this);
	FloatWrapperList.Add(NewWrapper);

	OnRep_FloatWrapperList();
}

void UAttributeBasedComponent::OnRep_BooleanWrapperList()
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

void UAttributeBasedComponent::OnRep_IntegerWrapperList()
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

void UAttributeBasedComponent::OnRep_FloatWrapperList()
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

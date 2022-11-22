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

bool UAttributeBasedComponent::RemoveAttribute(FName InName)
{
	return AttributeMap.Remove(InName) == 1;
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
	IAttributeCarrierInterface::OnRep_BooleanWrapperList();
}

void UAttributeBasedComponent::OnRep_IntegerWrapperList()
{
	IAttributeCarrierInterface::OnRep_IntegerWrapperList();
}

void UAttributeBasedComponent::OnRep_FloatWrapperList()
{
	IAttributeCarrierInterface::OnRep_FloatWrapperList();
}

#include "UnrealIntegration/Component/AttributeBasedComponent.h"

#include "AttributeSystem/Attribute/AttributeBase.h"

UAttributeBasedComponent::UAttributeBasedComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

bool UAttributeBasedComponent::AddAttribute(const FAttributeCreateArgument& InArgument)
{
	return IAttributeCarrierInterface::AddAttribute(InArgument);
}

bool UAttributeBasedComponent::GetAttributeBoolValue(FName InName)
{
	return IAttributeCarrierInterface::GetAttributeBoolValue(InName);
}

int32 UAttributeBasedComponent::GetAttributeIntegerValue(FName InName)
{
	return IAttributeCarrierInterface::GetAttributeIntegerValue(InName);
}

float UAttributeBasedComponent::GetAttributeFloatValue(FName InName)
{
	return IAttributeCarrierInterface::GetAttributeFloatValue(InName);
}

bool UAttributeBasedComponent::RemoveAttribute(FName InName)
{
	return AttributeMap.Remove(InName) == 1;
}

bool UAttributeBasedComponent::OnAttributeAdded(TSharedPtr<FAttributeBase> InAttribute)
{
	if (!InAttribute.IsValid() || AttributeMap.Contains(InAttribute->GetName()))
	{
		return false;
	}

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

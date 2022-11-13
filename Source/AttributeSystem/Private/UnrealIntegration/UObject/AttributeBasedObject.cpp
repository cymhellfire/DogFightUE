#include "UnrealIntegration/UObject/AttributeBasedObject.h"

#include "AttributeSystem/Attribute/AttributeBase.h"

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

#include "UnrealIntegration/Interface/AttributeCarrierInterface.h"
#include "AttributeSystem/AttributeFunctionLibrary.h"
#include "AttributeSystem/Attribute/Attribute.h"

bool IAttributeCarrierInterface::AddAttribute(const FAttributeCreateArgument& InArgument)
{
	auto NewAttribute = FAttributeFunctionLibrary::CreateAttribute(InArgument);
	if (NewAttribute.IsValid())
	{
		// Add new attribute
		return OnAttributeAdded(NewAttribute);
	}

	return false;
}

bool IAttributeCarrierInterface::GetAttributeBoolValue(FName InName, bool& OutValue)
{
	auto Attribute = GetAttribute(InName);
	// Convert to expected type
	if (auto ConvertedAttr = StaticCastSharedPtr<FAttributeBoolean>(Attribute))
	{
		OutValue = ConvertedAttr->GetValue();
		return true;
	}

	return false;
}

bool IAttributeCarrierInterface::GetAttributeIntegerValue(FName InName, int32& OutValue)
{
	auto Attribute = GetAttribute(InName);
	// Convert to expected type
	if (auto ConvertedAttr = StaticCastSharedPtr<FAttributeInteger>(Attribute))
	{
		OutValue = ConvertedAttr->GetValue();
		return true;
	}

	return false;
}

bool IAttributeCarrierInterface::GetAttributeFloatValue(FName InName, float& OutValue)
{
	auto Attribute = GetAttribute(InName);
	// Convert to expected type
	if (auto ConvertedAttr = StaticCastSharedPtr<FAttributeFloat>(Attribute))
	{
		OutValue = ConvertedAttr->GetValue();
		return true;
	}

	return false;
}

TArray<TSharedPtr<FAttributeBase>> IAttributeCarrierInterface::GetAttributesByDataType(EAttributeDataType InDataType)
{
	TArray<TSharedPtr<FAttributeBase>> Result;
	auto Candidates = GetAllAttributes();
	for (auto Attr : Candidates)
	{
		if (Attr->GetDataType() == InDataType)
		{
			Result.AddUnique(Attr);
		}
	}

	return Result;
}

TArray<TSharedPtr<FAttributeBase>> IAttributeCarrierInterface::GetAttributesByTags(const TArray<FName>& InTags, bool bMatchAll)
{
	TArray<TSharedPtr<FAttributeBase>> Result;
	auto Candidates = GetAllAttributes();
	if (bMatchAll)
	{
		for (auto Attr : Candidates)
		{
			if (Attr->HasAllTags(InTags))
			{
				Result.Add(Attr);
			}
		}
	}
	else
	{
		for (auto Attr : Candidates)
		{
			if (Attr->HasAnyTag(InTags))
			{
				Result.Add(Attr);
			}
		}
	}

	return Result;
}

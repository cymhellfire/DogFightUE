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

bool IAttributeCarrierInterface::GetAttributeBoolValue(FName InName)
{
	auto Attribute = GetAttribute(InName);
	// Convert to expected type
	if (auto ConvertedAttr = StaticCastSharedPtr<FAttributeBoolean>(Attribute))
	{
		return ConvertedAttr->GetValue();
	}

	return false;
}

int32 IAttributeCarrierInterface::GetAttributeIntegerValue(FName InName)
{
	auto Attribute = GetAttribute(InName);
	// Convert to expected type
	if (auto ConvertedAttr = StaticCastSharedPtr<FAttributeInteger>(Attribute))
	{
		return ConvertedAttr->GetValue();
	}

	return 0;
}

float IAttributeCarrierInterface::GetAttributeFloatValue(FName InName)
{
	auto Attribute = GetAttribute(InName);
	// Convert to expected type
	if (auto ConvertedAttr = StaticCastSharedPtr<FAttributeFloat>(Attribute))
	{
		return ConvertedAttr->GetValue();
	}

	return 0.f;
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

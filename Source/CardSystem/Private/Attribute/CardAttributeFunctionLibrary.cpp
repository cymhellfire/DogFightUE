#include "Attribute/CardAttributeFunctionLibrary.h"

#include "AttributeSystem/AttributeFunctionLibrary.h"
#include "AttributeSystem/Attribute/Attribute.h"
#include "AttributeSystem/Attribute/AttributeCommon.h"
#include "Card/Card.h"

bool FCardAttributeFunctionLibrary::CreateBoolAttributeForCard(UCard* InCard, FName InName, bool InValue)
{
	if (InCard == nullptr)
	{
		return false;
	}

	FAttributeCreateArgument CreateArgument;
	CreateArgument.AttrName = InName;
	CreateArgument.DataType = ADT_Boolean;

	auto NewAttribute = FAttributeFunctionLibrary::CreateAttribute(CreateArgument);
	// Convert to expected type
	auto ConvertedAttr = StaticCastSharedPtr<TAttributeBase<bool>>(NewAttribute);

	return AddAttributeToCard(InCard, ConvertedAttr, InValue);
}

bool FCardAttributeFunctionLibrary::CreateIntegerAttributeForCard(UCard* InCard, FName InName, int32 InValue)
{
	if (InCard == nullptr)
	{
		return false;
	}

	FAttributeCreateArgument CreateArgument;
	CreateArgument.AttrName = InName;
	CreateArgument.DataType = ADT_Integer;

	auto NewAttribute = FAttributeFunctionLibrary::CreateAttribute(CreateArgument);
	// Convert to expected type
	auto ConvertedAttr = StaticCastSharedPtr<TAttributeBase<int32>>(NewAttribute);

	return AddAttributeToCard(InCard, ConvertedAttr, InValue);
}

bool FCardAttributeFunctionLibrary::CreateFloatAttributeForCard(UCard* InCard, FName InName, float InValue)
{
	if (InCard == nullptr)
	{
		return false;
	}

	FAttributeCreateArgument CreateArgument;
	CreateArgument.AttrName = InName;
	CreateArgument.DataType = ADT_Float;

	auto NewAttribute = FAttributeFunctionLibrary::CreateAttribute(CreateArgument);
	// Convert to expected type
	auto ConvertedAttr = StaticCastSharedPtr<TAttributeBase<float>>(NewAttribute);

	return AddAttributeToCard(InCard, ConvertedAttr, InValue);
}

template <typename T>
bool FCardAttributeFunctionLibrary::AddAttributeToCard(UCard* InCard, TSharedPtr<TAttributeBase<T>> InAttribute,
	T InValue)
{
	if (InAttribute.IsValid())
	{
		// Set initial value
		InAttribute->SetRawValue(InValue);

		// Add attribute to card
		return InCard->AddAttribute(InAttribute);
	}

	return false;
}

bool FCardAttributeFunctionLibrary::GetBoolAttributeValueFromCard(UCard* InCard, FName InName, bool& OutValue)
{
	auto Attribute = GetAttributeFromCard<bool>(InCard, InName);
	if (Attribute == nullptr || !Attribute.IsValid())
	{
		return false;
	}

	OutValue = Attribute.Pin()->GetValue();
	return true;
}

bool FCardAttributeFunctionLibrary::GetIntegerAttributeValueFromCard(UCard* InCard, FName InName, int32& OutValue)
{
	auto Attribute = GetAttributeFromCard<int32>(InCard, InName);
	if (Attribute == nullptr || !Attribute.IsValid())
	{
		return false;
	}

	OutValue = Attribute.Pin()->GetValue();
	return true;
}

bool FCardAttributeFunctionLibrary::GetFloatAttributeValueFromCard(UCard* InCard, FName InName, float& OutValue)
{
	auto Attribute = GetAttributeFromCard<float>(InCard, InName);
	if (Attribute == nullptr || !Attribute.IsValid())
	{
		return false;
	}

	OutValue = Attribute.Pin()->GetValue();
	return true;
}

template <typename T>
TWeakPtr<TAttributeBase<T>> FCardAttributeFunctionLibrary::GetAttributeFromCard(UCard* InCard, FName InName)
{
	if (InCard == nullptr)
	{
		return nullptr;
	}

	auto Attribute = InCard->GetAttribute(InName);
	if (Attribute == nullptr || !Attribute.IsValid())
	{
		return nullptr;
	}

	// Convert to excepted type
	auto ConvertedAttribute = StaticCastSharedPtr<TAttributeBase<T>>(Attribute.Pin());
	if (ConvertedAttribute.IsValid())
	{
		return ConvertedAttribute;
	}

	return nullptr;
}

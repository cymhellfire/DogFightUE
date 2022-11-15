#include "UnrealIntegration/Interface/AttributeCarrierInterface.h"

#include "AttributeSystem.h"
#include "AttributeSystem/AttributeFunctionLibrary.h"
#include "AttributeSystem/Attribute/Attribute.h"
#include "UnrealIntegration/Interface/AttributeModifierCarrierInterface.h"
#include "UnrealIntegration/UObject/AttributeModifierDescObject.h"

bool IAttributeCarrierInterface::AddAttribute(const FAttributeCreateArgument& InArgument)
{
	auto NewAttribute = FAttributeFunctionLibrary::CreateAttribute(InArgument);
	if (NewAttribute && NewAttribute.IsValid())
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

bool IAttributeCarrierInterface::AddModifierObject(TScriptInterface<IAttributeModifierCarrierInterface> InModifierObject)
{
	if (InModifierObject == nullptr)
	{
		return false;
	}

	// Try to apply modifier
	if (AddAttributeModifier(InModifierObject->GetModifier()))
	{
		// Notify new modifier object added
		OnModifierInterfaceAdded(InModifierObject.GetInterface());
		OnModifierObjectAdded(InModifierObject.GetObject());

		// Generate new description and notify
		auto NewDesc = InModifierObject->GenerateDescObject(GetSubobjectCarrier());
		if (NewDesc)
		{
			OnModifierDescObjectAdded(InModifierObject.GetObject(), NewDesc);
		}

		return true;
	}

	return false;
}

bool IAttributeCarrierInterface::RemoveModifierObject(TScriptInterface<IAttributeModifierCarrierInterface> InModifierObject)
{
	if (InModifierObject == nullptr)
	{
		return false;
	}

	// Skip modifier that is not applied yet
	if (!IsModifierObjectApplied(InModifierObject.GetInterface()))
	{
		UE_LOG(LogAttributeSystem, Warning, TEXT("[AttributeCarrier] Modifier must be applied before remove."));
		return false;
	}

	// Remove modifier
	InModifierObject->RemoveFromTarget();

	// Notify modifier object removed
	OnModifierInterfaceRemoved(InModifierObject.GetInterface());
	OnModifierObjectRemoved(InModifierObject.GetObject());

	return true;
}

bool IAttributeCarrierInterface::AddAttributeModifier(TSharedPtr<FAttributeModifierBase> InModifier)
{
	if (!InModifier.IsValid())
	{
		return false;
	}

	// Filter attributes by modifier data type
	auto CandidateList = GetAttributesByDataType(InModifier->GetDataType());
	if (CandidateList.Num() == 0)
	{
		UE_LOG(LogAttributeSystem, Warning, TEXT("[AttributeCarrier] No attribute matches the data type [%s] from modifier."),
			*UEnum::GetValueAsString<EAttributeDataType>(InModifier->GetDataType()));
		return false;
	}

	// Pick random one from list if multiple attributes matched the data type
	TSharedPtr<FAttributeBase> TargetAttribute = CandidateList[FMath::RandRange(0, CandidateList.Num() - 1)];
	TargetAttribute->AddModifier(InModifier);

	return true;
}

bool IAttributeCarrierInterface::IsModifierObjectApplied(IAttributeModifierCarrierInterface* InModifier) const
{
	return GetAllModifierObjects().Contains(InModifier);
}

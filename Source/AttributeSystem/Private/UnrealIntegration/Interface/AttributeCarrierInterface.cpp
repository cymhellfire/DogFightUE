#include "UnrealIntegration/Interface/AttributeCarrierInterface.h"

#include "AttributeSystem.h"
#include "AttributeSystem/AttributeFunctionLibrary.h"
#include "AttributeSystem/Attribute/Attribute.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "UnrealIntegration/DataWrapper/AttributeWrapper.h"
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
	TSharedPtr<FAttributeBase> AppliedAttribute = nullptr;
	if (AddAttributeModifier(InModifierObject->GetModifier(), AppliedAttribute))
	{
		// Notify new modifier object added
		OnModifierInterfaceAdded(InModifierObject.GetInterface());
		OnModifierObjectAdded(InModifierObject.GetObject());

		// Generate new description and notify
		auto NewDesc = InModifierObject->GetDescObject(GetSubobjectCarrier());
		if (NewDesc)
		{
			OnModifierDescObjectAdded(InModifierObject.GetObject(), NewDesc);
		}

		// Add new description object to property wrapper
		UpdateDescObjectToProperty(AppliedAttribute, NewDesc, true);

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
	TSharedPtr<FAttributeBase> TargetAttr;
	InModifierObject->RemoveFromTarget(TargetAttr);

	// Remove description object from property wrapper
	UpdateDescObjectToProperty(TargetAttr, InModifierObject->GetDescObject(GetSubobjectCarrier()), false);

	// Notify modifier object removed
	OnModifierInterfaceRemoved(InModifierObject.GetInterface());
	OnModifierObjectRemoved(InModifierObject.GetObject());

	return true;
}

bool IAttributeCarrierInterface::AddAttributeModifier(TSharedPtr<FAttributeModifierBase> InModifier, TSharedPtr<FAttributeBase>& OutAttribute)
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

	OutAttribute = TargetAttribute;
	return true;
}

bool IAttributeCarrierInterface::IsModifierObjectApplied(IAttributeModifierCarrierInterface* InModifier) const
{
	return GetAllModifierObjects().Contains(InModifier);
}

void IAttributeCarrierInterface::UpdateDescObjectToProperty(TSharedPtr<FAttributeBase> AppliedAttribute,
	UAttributeModifierDescObject* InDescObject, bool bAdd)
{
	// Record new modifier object to wrapper Property
	UObject* ThisObject = ThisAsObject();
	if (ThisObject)
	{
		UClass* MyClass = ThisObject->GetClass();
		// Here is a rule, the wrapper property should have the same name as inner FAttributeBase instance.
		if (FProperty* Property = MyClass->FindPropertyByName(AppliedAttribute->GetName()))
		{
			bool bFailed = false;
			void* PropertyAddr = Property->ContainerPtrToValuePtr<void>(ThisObject);
			switch(AppliedAttribute->GetDataType())
			{
			case ADT_Boolean:
				if (FAttributeBooleanWrapper* BooleanWrapper = static_cast<FAttributeBooleanWrapper*>(PropertyAddr))
				{
					if (bAdd)
						BooleanWrapper->AppliedModifierDesc.Add(InDescObject);
					else
						BooleanWrapper->AppliedModifierDesc.Remove(InDescObject);
				}
				else
				{
					bFailed = true;
				}
				break;
			case ADT_Integer:
				if (FAttributeIntegerWrapper* IntegerWrapper = static_cast<FAttributeIntegerWrapper*>(PropertyAddr))
				{
					if (bAdd)
						IntegerWrapper->AppliedModifierDesc.Add(InDescObject);
					else
						IntegerWrapper->AppliedModifierDesc.Remove(InDescObject);
				}
				else
				{
					bFailed = true;
				}
				break;
			case ADT_Float:
				if (FAttributeFloatWrapper* FloatWrapper = static_cast<FAttributeFloatWrapper*>(PropertyAddr))
				{
					if (bAdd)
						FloatWrapper->AppliedModifierDesc.Add(InDescObject);
					else
						FloatWrapper->AppliedModifierDesc.Remove(InDescObject);
				}
				else
				{
					bFailed = true;
				}
				break;
			case ADT_None:
			default: ;
			}

			if (bFailed)
			{
				UE_LOG(LogAttributeSystem, Error, TEXT("[AttributeCarrierInterface] Data type mismatch. Expected property %s type is %s"),
					*AppliedAttribute->GetName().ToString(), *UEnum::GetValueAsString<EAttributeDataType>(AppliedAttribute->GetDataType()));
			}
		}
		else
		{
			UE_LOG(LogAttributeSystem, Error,
				TEXT("[AttributeCarrierInterface] Cannot find UPROPERTY with name %s. Modifier information cannot be synced."),
				*AppliedAttribute->GetName().ToString());
		}
	}
}

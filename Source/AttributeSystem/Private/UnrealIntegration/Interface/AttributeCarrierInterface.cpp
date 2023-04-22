#include "UnrealIntegration/Interface/AttributeCarrierInterface.h"

#include "AttributeSystem.h"
#include "AttributeSystem/AttributeFunctionLibrary.h"
#include "AttributeSystem/Attribute/Attribute.h"
#include "Math/MathHelper.h"
#include "UnrealIntegration/Interface/AttributeModifierCarrierInterface.h"
#include "UnrealIntegration/UObject/AttributeModifierDescObject.h"

bool IAttributeCarrierInterface::AddAttribute(const FAttributeCreateArgument& InArgument)
{
	auto NewAttribute = FAttributeFunctionLibrary::CreateAttribute(InArgument);
	if (NewAttribute && NewAttribute.IsValid())
	{
		// Add new attribute
		if (OnAttributeAdded(NewAttribute))
		{
			// Create wrapper object for attributes that added succeed
			CreateWrapperObjectForAttribute(NewAttribute);

			return true;
		}
	}

	return false;
}

bool IAttributeCarrierInterface::AddBooleanAttribute(FName InName, bool InitValue, FString Tags)
{
	TArray<FString> TagArray;
	if (!Tags.IsEmpty())
	{
		Tags.ParseIntoArray(TagArray, TEXT(","));
	}

	FAttributeCreateArgument CreateArgument;
	CreateArgument.AttrName = InName;
	CreateArgument.DataType = ADT_Boolean;
	CreateArgument.Tags = TagArray;
	CreateArgument.InitBooleanValue = InitValue;

	return AddAttribute(CreateArgument);
}

bool IAttributeCarrierInterface::AddIntegerAttribute(FName InName, int32 InitValue, FString Tags)
{
	TArray<FString> TagArray;
	if (!Tags.IsEmpty())
	{
		Tags.ParseIntoArray(TagArray, TEXT(","));
	}

	FAttributeCreateArgument CreateArgument;
	CreateArgument.AttrName = InName;
	CreateArgument.DataType = ADT_Integer;
	CreateArgument.Tags = TagArray;
	CreateArgument.InitIntegerValue = InitValue;

	return AddAttribute(CreateArgument);
}

bool IAttributeCarrierInterface::AddFloatAttribute(FName InName, float InitValue, FString Tags)
{
	TArray<FString> TagArray;
	if (!Tags.IsEmpty())
	{
		Tags.ParseIntoArray(TagArray, TEXT(","));
	}

	FAttributeCreateArgument CreateArgument;
	CreateArgument.AttrName = InName;
	CreateArgument.DataType = ADT_Float;
	CreateArgument.Tags = TagArray;
	CreateArgument.InitFloatValue = InitValue;

	return AddAttribute(CreateArgument);
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

	// Clear the pointer before apply
	OutAttribute.Reset();

	// Filter attributes by modifier data type
	auto CandidateList = GetAttributesByDataType(InModifier->GetDataType());
	if (CandidateList.Num() == 0)
	{
		UE_LOG(LogAttributeSystem, Warning, TEXT("[AttributeCarrier] No attribute matches the data type [%s] from modifier."),
			*UEnum::GetValueAsString<EAttributeDataType>(InModifier->GetDataType()));
		return false;
	}

	// Randomize the candidate
	FMathHelper::RandomizeArray(CandidateList);

	// Iterate through all candidate and pick the first one fit apply rule
	for (int32 i = 0; i < CandidateList.Num(); ++i)
	{
		// Check the apply rule
		if (InModifier->CanApply(CandidateList[i]))
		{
			CandidateList[i]->AddModifier(InModifier);
			OutAttribute = CandidateList[i];
			break;
		}
	}

	if (!OutAttribute.IsValid())
	{
		bool bAdded = false;
		// Check if we need add the missing attribute
		if (bAddMissingAttributeForModifier)
		{
			FName NewAttributeName;
			EAttributeDataType AttributeDataType;
			if (InModifier->GetDesiredName(NewAttributeName) && InModifier->GetDesiredDataType(AttributeDataType))
			{
				// Construct the attribute create argument based on modifier information
				auto AttributeArgument = FAttributeCreateArgument();
				AttributeArgument.AttrName = NewAttributeName;
				AttributeArgument.DataType = AttributeDataType;

				if (AddAttribute(AttributeArgument))
				{
					// Get the new added attribute and apply modifier
					OutAttribute = GetAttribute(NewAttributeName);
					if (OutAttribute.IsValid())
					{
						OutAttribute->AddModifier(InModifier);
					}
					bAdded = true;
				}
				else
				{
					UE_LOG(LogAttributeSystem, Error, TEXT("[AttributeCarrier] Cannot auto add attribute %s[Type:%d] by modifier."),
						*NewAttributeName.ToString(), AttributeDataType);
				}
			}
		}

		if (!bAdded)
		{
			return false;
		}
	}

	return true;
}

bool IAttributeCarrierInterface::IsModifierObjectApplied(IAttributeModifierCarrierInterface* InModifier) const
{
	return GetAllModifierObjects().Contains(InModifier);
}

void IAttributeCarrierInterface::CreateWrapperObjectForAttribute(TSharedPtr<FAttributeBase> InAttribute)
{
	EAttributeDataType DataType = InAttribute->GetDataType();
	switch(DataType)
	{
	case ADT_Boolean:
		if (auto BooleanAttribute = StaticCastSharedPtr<FAttributeBoolean>(InAttribute))
		{
			UAttributeBooleanWrapperObject* NewWrapper = FAttributeWrapperObjectHelper::CreateWrapperObjectForBooleanAttribute(
				ThisAsObject(), BooleanAttribute, [this](TSharedPtr<FAttributeBase> Attribute)
				{
					if (auto ConvertAttr = StaticCastSharedPtr<FAttributeBoolean>(Attribute))
					{
						if (auto Wrapper = GetBooleanAttributeWrapperByName(Attribute->GetName()))
						{
							Wrapper->SetValue(ConvertAttr->GetValue());
						}
					}
				});

			// Notify the derived class
			OnBooleanAttributeWrapperObjectCreated(NewWrapper);
		}
		break;
	case ADT_Integer:
		if (auto IntegerAttribute = StaticCastSharedPtr<FAttributeInteger>(InAttribute))
		{
			UAttributeIntegerWrapperObject* NewWrapper = FAttributeWrapperObjectHelper::CreateWrapperObjectForIntegerAttribute(
				ThisAsObject(), IntegerAttribute, [this](TSharedPtr<FAttributeBase> Attribute)
				{
					if (auto ConvertAttr = StaticCastSharedPtr<FAttributeInteger>(Attribute))
					{
						if (auto Wrapper = GetIntegerAttributeWrapperByName(Attribute->GetName()))
						{
							Wrapper->SetValue(ConvertAttr->GetValue());
						}
					}
				});

			// Notify the derived class
			OnIntegerAttributeWrapperObjectCreated(NewWrapper);
		}
		break;
	case ADT_Float:
		if (auto FloatAttribute = StaticCastSharedPtr<FAttributeFloat>(InAttribute))
		{
			UAttributeFloatWrapperObject* NewWrapper = FAttributeWrapperObjectHelper::CreateWrapperObjectForFloatAttribute(
				ThisAsObject(), FloatAttribute, [this](TSharedPtr<FAttributeBase> Attribute)
				{
					if (auto ConvertAttr = StaticCastSharedPtr<FAttributeFloat>(Attribute))
					{
						if (auto Wrapper = GetFloatAttributeWrapperByName(Attribute->GetName()))
						{
							Wrapper->SetValue(ConvertAttr->GetValue());
						}
					}
				});

			// Notify the derived class
			OnFloatAttributeWrapperObjectCreated(NewWrapper);
		}
		break;
	case ADT_None:
	default: ;
	}
}

void IAttributeCarrierInterface::UpdateDescObjectToProperty(TSharedPtr<FAttributeBase> AppliedAttribute,
                                                            UAttributeModifierDescObject* InDescObject, bool bAdd)
{
	bool bFailed = false;
	const FName AttributeName = AppliedAttribute->GetName();
	switch(AppliedAttribute->GetDataType())
	{
	case ADT_Boolean:
		if (auto Attribute = GetBooleanAttributeWrapperByName(AttributeName))
		{
			if (bAdd)
				Attribute->AddDescObject(InDescObject);
			else
				Attribute->RemoveDescObject(InDescObject);
		}
		else
		{
			bFailed = true;
		}
		break;
	case ADT_Integer:
		if (auto Attribute = GetIntegerAttributeWrapperByName(AttributeName))
		{
			if (bAdd)
				Attribute->AddDescObject(InDescObject);
			else
				Attribute->RemoveDescObject(InDescObject);
		}
		else
		{
			bFailed = true;
		}
		break;
	case ADT_Float:
		if (auto Attribute = GetFloatAttributeWrapperByName(AttributeName))
		{
			if (bAdd)
				Attribute->AddDescObject(InDescObject);
			else
				Attribute->RemoveDescObject(InDescObject);
		}
		else
		{
			bFailed = true;
		}
		break;
	case ADT_None:
	default:
		UE_LOG(LogAttributeSystem, Error, TEXT("[AttributeCarrierInterface] Attribute %s's data type invalid."),
			*AppliedAttribute->GetName().ToString());;
	}

	if (bFailed)
	{
		UE_LOG(LogAttributeSystem, Error, TEXT("[AttributeCarrierInterface] Cannot find wrapper member with name %s."),
			*AppliedAttribute->GetName().ToString());
	}
}

UAttributeBooleanWrapperObject* IAttributeCarrierInterface::GetBooleanAttributeWrapperByName(FName InName)
{
	auto WrapperPtr = BooleanWrapperMap.Find(InName);
	return (WrapperPtr && WrapperPtr->IsValid()) ? WrapperPtr->Get() : nullptr;
}

UAttributeIntegerWrapperObject* IAttributeCarrierInterface::GetIntegerAttributeWrapperByName(FName InName)
{
	auto WrapperPtr = IntegerWrapperMap.Find(InName);
	return (WrapperPtr && WrapperPtr->IsValid()) ? WrapperPtr->Get() : nullptr;
}

UAttributeFloatWrapperObject* IAttributeCarrierInterface::GetFloatAttributeWrapperByName(FName InName)
{
	auto WrapperPtr = FloatWrapperMap.Find(InName);
	return (WrapperPtr && WrapperPtr->IsValid()) ? WrapperPtr->Get() : nullptr;
}

void IAttributeCarrierInterface::ValidateWrapperObjectMap(EAttributeDataType InDataType, TArray<FName>* OutInvalidKeys)
{
	TArray<FName> InvalidKeys;
	switch (InDataType)
	{
	case ADT_Boolean:
		for (auto& Record : BooleanWrapperMap)
		{
			if (Record.Value == nullptr || !Record.Value.IsValid())
			{
				InvalidKeys.Add(Record.Key);
			}
		}

		// Remove all invalid keys
		for (auto Key : InvalidKeys)
		{
			BooleanWrapperMap.Remove(Key);
		}
		break;
	case ADT_Integer:
		for (auto& Record : IntegerWrapperMap)
		{
			if (Record.Value == nullptr || !Record.Value.IsValid())
			{
				InvalidKeys.Add(Record.Key);
			}
		}

		// Remove all invalid keys
		for (auto Key : InvalidKeys)
		{
			IntegerWrapperMap.Remove(Key);
		}
		break;
	case ADT_Float:
		for (auto& Record : FloatWrapperMap)
		{
			if (Record.Value == nullptr || !Record.Value.IsValid())
			{
				InvalidKeys.Add(Record.Key);
			}
		}

		// Remove all invalid keys
		for (auto Key : InvalidKeys)
		{
			FloatWrapperMap.Remove(Key);
		}
		break;
	case ADT_None:
	default: ;
	}

#if ATTR_DETAIL_LOG
	if (OutInvalidKeys)
	{
		OutInvalidKeys->Empty();
		OutInvalidKeys->Append(InvalidKeys);
	}
#endif
}

void IAttributeCarrierInterface::OnRep_BooleanWrapperList()
{
#if ATTR_DETAIL_LOG
	TArray<UAttributeBooleanWrapperObject*> NewAddedWrappers;
#endif
	const ENetRole LocalRole = GetNetRole();
	const auto WrapperList = GetAllBooleanAttributeWrappers();
	for (auto Wrapper : WrapperList)
	{
		if (!Wrapper)
			continue;

		const FName AttributeName = Wrapper->GetAttributeName();
		// Add new synced wrapper to map
		if (!BooleanWrapperMap.Contains(AttributeName))
		{
			BooleanWrapperMap.Add(AttributeName, Wrapper);

			//Notify the client
			if (LocalRole != ENetRole::ROLE_Authority)
			{
				Sync_OnBooleanWrapperAdded(Wrapper);
			}
#if ATTR_DETAIL_LOG
			NewAddedWrappers.Add(Wrapper);
#endif
		}
	}

#if ATTR_DETAIL_LOG
	if (NewAddedWrappers.Num() > 0)
	{
		const FString ObjName = ThisAsObject()->GetName();
		const FString NetRoleStr = TO_NET_ROLE_STR(LocalRole);
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
		const FString ObjName = ThisAsObject()->GetName();
		const FString NetRoleStr = TO_NET_ROLE_STR(LocalRole);
		for (auto Key : InvalidKeys)
		{
			UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] Attribute %s removed."), *NetRoleStr, *ObjName, *Key.ToString());
		}
	}
#else
	ValidateWrapperObjectMap(ADT_Boolean);
#endif
}

void IAttributeCarrierInterface::OnRep_IntegerWrapperList()
{
#if ATTR_DETAIL_LOG
	TArray<UAttributeIntegerWrapperObject*> NewAddedWrappers;
#endif
	const ENetRole LocalRole = GetNetRole();
	const auto WrapperList = GetAllIntegerAttributeWrappers();
	for (auto Wrapper : WrapperList)
	{
		if (!Wrapper)
			continue;

		const FName AttributeName = Wrapper->GetAttributeName();
		// Add new synced wrapper to map
		if (!IntegerWrapperMap.Contains(AttributeName))
		{
			IntegerWrapperMap.Add(AttributeName, Wrapper);

			//Notify the client
			if (LocalRole != ENetRole::ROLE_Authority)
			{
				Sync_OnIntegerWrapperAdded(Wrapper);
			}
#if ATTR_DETAIL_LOG
			NewAddedWrappers.Add(Wrapper);
#endif
		}
	}

#if ATTR_DETAIL_LOG
	if (NewAddedWrappers.Num() > 0)
	{
		const FString ObjName = ThisAsObject()->GetName();
		const FString NetRoleStr = TO_NET_ROLE_STR(LocalRole);
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
		const FString ObjName = ThisAsObject()->GetName();
		const FString NetRoleStr = TO_NET_ROLE_STR(LocalRole);
		for (auto Key : InvalidKeys)
		{
			UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] Attribute %s removed."), *NetRoleStr, *ObjName, *Key.ToString());
		}
	}
#else
	ValidateWrapperObjectMap(ADT_Integer);
#endif
}

void IAttributeCarrierInterface::OnRep_FloatWrapperList()
{
#if ATTR_DETAIL_LOG
	TArray<UAttributeFloatWrapperObject*> NewAddedWrappers;
#endif
	const ENetRole LocalRole = GetNetRole();
	const auto WrapperList = GetAllFloatAttributeWrappers();
	for (auto Wrapper : WrapperList)
	{
		if (!Wrapper)
			return;

		const FName AttributeName = Wrapper->GetAttributeName();
		// Add new synced wrapper to map
		if (!FloatWrapperMap.Contains(AttributeName))
		{
			FloatWrapperMap.Add(AttributeName, Wrapper);

			//Notify the client
			if (LocalRole != ENetRole::ROLE_Authority)
			{
				Sync_OnFloatWrapperAdded(Wrapper);
			}
#if ATTR_DETAIL_LOG
			NewAddedWrappers.Add(Wrapper);
#endif
		}
	}

#if ATTR_DETAIL_LOG
	if (NewAddedWrappers.Num() > 0)
	{
		const FString ObjName = ThisAsObject()->GetName();
		const FString NetRoleStr = TO_NET_ROLE_STR(LocalRole);
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
		const FString ObjName = ThisAsObject()->GetName();
		const FString NetRoleStr = TO_NET_ROLE_STR(LocalRole);
		for (auto Key : InvalidKeys)
		{
			UE_LOG(LogAttributeSystem, Log, TEXT("%s: [%s] Attribute %s removed."), *NetRoleStr, *ObjName, *Key.ToString());
		}
	}
#else
	ValidateWrapperObjectMap(ADT_Float);
#endif
}

#include "Card/CardDescObject.h"
#include "CardSystem.h"
#include "Net/UnrealNetwork.h"
#include "AttributeSystem/Attribute/Attribute.h"
#include "AttributeSystem/Attribute/AttributeBase.h"
#include "Engine/ActorChannel.h"
#include "UnrealIntegration/DataWrapper/AttributeWrapperObject.h"

void UCardDescObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(UCardDescObject, BooleanWrapperList, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UCardDescObject, IntegerWrapperList, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UCardDescObject, FloatWrapperList, SharedParams);
}

bool UCardDescObject::ReplicateModifierDescObjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateModifierDescObjects(Channel, Bunch, RepFlags);

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

bool UCardDescObject::OnAttributeAdded(TSharedPtr<FAttributeBase> InAttribute)
{
	bool Result = Super::OnAttributeAdded(InAttribute);
	if (!Result)
	{
		return Result;
	}

	// Create wrapper for new added attribute
	const FName AttributeName = InAttribute->GetName();
	EAttributeDataType DataType = InAttribute->GetDataType();
	switch(DataType)
	{
	case ADT_Boolean:
		if (auto BooleanAttribute = StaticCastSharedPtr<FAttributeBoolean>(InAttribute))
		{
			UAttributeBooleanWrapperObject* NewWrapper = NewObject<UAttributeBooleanWrapperObject>(this, NAME_None, RF_Transient);
			NewWrapper->SetAttributeName(AttributeName);
			NewWrapper->SetBaseValue(BooleanAttribute->GetRawValue());
			NewWrapper->SetValue(BooleanAttribute->GetValue());
			BooleanAttribute->OnValueChanged.AddLambda([this](TSharedPtr<FAttributeBase> Attribute)
			{
				if (auto ConvertAttr = StaticCastSharedPtr<FAttributeBoolean>(Attribute))
				{
					if (auto Wrapper = GetBooleanAttributeWrapperByName(Attribute->GetName()))
					{
						Wrapper->SetValue(ConvertAttr->GetValue());
					}
				}
			});

			MARK_PROPERTY_DIRTY_FROM_NAME(UCardDescObject, BooleanWrapperList, this);
			BooleanWrapperList.Add(NewWrapper);

			OnRep_BooleanWrapperList();
		}
		break;
	case ADT_Integer:
		if (auto IntegerAttribute = StaticCastSharedPtr<FAttributeInteger>(InAttribute))
		{
			UAttributeIntegerWrapperObject* NewWrapper = NewObject<UAttributeIntegerWrapperObject>(this, NAME_None, RF_Transient);
			NewWrapper->SetAttributeName(AttributeName);
			NewWrapper->SetBaseValue(IntegerAttribute->GetRawValue());
			NewWrapper->SetValue(IntegerAttribute->GetValue());
			IntegerAttribute->OnValueChanged.AddLambda([this](TSharedPtr<FAttributeBase> Attribute)
			{
				if (auto ConvertAttr = StaticCastSharedPtr<FAttributeInteger>(Attribute))
				{
					if (auto Wrapper = GetIntegerAttributeWrapperByName(Attribute->GetName()))
					{
						Wrapper->SetValue(ConvertAttr->GetValue());
					}
				}
			});

			MARK_PROPERTY_DIRTY_FROM_NAME(UCardDescObject, IntegerWrapperList, this);
			IntegerWrapperList.Add(NewWrapper);

			OnRep_IntegerWrapperList();
		}
		break;
	case ADT_Float:
		if (auto FloatAttribute = StaticCastSharedPtr<FAttributeFloat>(InAttribute))
		{
			UAttributeFloatWrapperObject* NewWrapper = NewObject<UAttributeFloatWrapperObject>(this, NAME_None, RF_Transient);
			NewWrapper->SetAttributeName(AttributeName);
			NewWrapper->SetBaseValue(FloatAttribute->GetRawValue());
			NewWrapper->SetValue(FloatAttribute->GetValue());
			FloatAttribute->OnValueChanged.AddLambda([this](TSharedPtr<FAttributeBase> Attribute)
			{
				if (auto ConvertAttr = StaticCastSharedPtr<FAttributeFloat>(Attribute))
				{
					if (auto Wrapper = GetFloatAttributeWrapperByName(Attribute->GetName()))
					{
						Wrapper->SetValue(ConvertAttr->GetValue());
					}
				}
			});

			MARK_PROPERTY_DIRTY_FROM_NAME(UCardDescObject, FloatWrapperList, this);
			FloatWrapperList.Add(NewWrapper);

			OnRep_FloatWrapperList();
		}
		break;
	case ADT_None:
	default: ;
	}

	return true;
}

UAttributeBooleanWrapperObject* UCardDescObject::GetBooleanAttributeWrapperByName(FName InName)
{
	auto WrapperPtr = BooleanWrapperMap.Find(InName);
	return (WrapperPtr && WrapperPtr->IsValid()) ? WrapperPtr->Get() : nullptr;
}

UAttributeIntegerWrapperObject* UCardDescObject::GetIntegerAttributeWrapperByName(FName InName)
{
	auto WrapperPtr = IntegerWrapperMap.Find(InName);
	return (WrapperPtr && WrapperPtr->IsValid()) ? WrapperPtr->Get() : nullptr;
}

UAttributeFloatWrapperObject* UCardDescObject::GetFloatAttributeWrapperByName(FName InName)
{
	auto WrapperPtr = FloatWrapperMap.Find(InName);
	return (WrapperPtr && WrapperPtr->IsValid()) ? WrapperPtr->Get() : nullptr;
}

void UCardDescObject::OnRep_BooleanWrapperList()
{
	for (auto Wrapper : BooleanWrapperList)
	{
		if (!Wrapper)
			continue;

		const FName AttributeName = Wrapper->GetAttributeName();
		// Add new synced wrapper to map
		if (!BooleanWrapperMap.Contains(AttributeName))
		{
			BooleanWrapperMap.Add(AttributeName, Wrapper);
			UE_LOG(LogCardSystem, Log, TEXT("[CardDescObject] New attribute %s added."), *AttributeName.ToString());
			UE_LOG(LogCardSystem, Log, TEXT("[CardDescObject] %s"), *Wrapper->ToString());
		}
	}
}

void UCardDescObject::OnRep_IntegerWrapperList()
{
	for (auto Wrapper : IntegerWrapperList)
	{
		if (!Wrapper)
			continue;

		const FName AttributeName = Wrapper->GetAttributeName();
		// Add new synced wrapper to map
		if (!IntegerWrapperMap.Contains(AttributeName))
		{
			IntegerWrapperMap.Add(AttributeName, Wrapper);
			UE_LOG(LogCardSystem, Log, TEXT("[CardDescObject] New attribute %s added."), *AttributeName.ToString());
			UE_LOG(LogCardSystem, Log, TEXT("[CardDescObject] %s"), *Wrapper->ToString());
		}
	}
}

void UCardDescObject::OnRep_FloatWrapperList()
{
	for (auto Wrapper : FloatWrapperList)
	{
		if (!Wrapper)
			return;

		const FName AttributeName = Wrapper->GetAttributeName();
		// Add new synced wrapper to map
		if (!FloatWrapperMap.Contains(AttributeName))
		{
			FloatWrapperMap.Add(AttributeName, Wrapper);
			UE_LOG(LogCardSystem, Log, TEXT("[CardDescObject] New attribute %s added."), *AttributeName.ToString());
			UE_LOG(LogCardSystem, Log, TEXT("[CardDescObject] %s"), *Wrapper->ToString());
		}
	}
}

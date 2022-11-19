#include "Card/CardDescObject.h"
#include "Net/UnrealNetwork.h"
#include "AttributeSystem/Attribute/Attribute.h"
#include "AttributeSystem/Attribute/AttributeBase.h"
#include "UnrealIntegration/DataWrapper/AttributeWrapperObject.h"
#include "UnrealIntegration/UObject/AttributeModifierDescObject.h"

void UCardDescObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
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
		}
		break;
	case ADT_Integer:
		if (auto IntegerAttribute = StaticCastSharedPtr<FAttributeInteger>(InAttribute))
		{
			UAttributeIntegerWrapperObject* NewWrapper = NewObject<UAttributeIntegerWrapperObject>(this, NMAE_None, RF_Transient);
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
	return WrapperPtr->IsValid() ? WrapperPtr->Get() : nullptr;
}

UAttributeIntegerWrapperObject* UCardDescObject::GetIntegerAttributeWrapperByName(FName InName)
{
	auto WrapperPtr = IntegerWrapperMap.Find(InName);
	return WrapperPtr->IsValid() ? WrapperPtr->Get() : nullptr;
}

UAttributeFloatWrapperObject* UCardDescObject::GetFloatAttributeWrapperByName(FName InName)
{
	auto WrapperPtr = FloatWrapperMap.Find(InName);
	return WrapperPtr->IsValid() ? WrapperPtr->Get() : nullptr;
}

void UCardDescObject::OnRep_BooleanWrapperList()
{
	for (auto Wrapper : BooleanWrapperList)
	{
		int32 Index = 0;
		for (auto Desc : Wrapper->AppliedModifierDesc)
		{
			UE_LOG(LogTemp, Log, TEXT("[TestAttribute] Modifier %d: Source = %s, Effect = %s"), Index, *Desc->GetSourceString(),
				*Desc->GetEffectString());
			Index++;
		}
	}
}

void UCardDescObject::OnRep_IntegerWrapperList()
{
	for (auto Wrapper : IntegerWrapperList)
	{
		int32 Index = 0;
		for (auto Desc : Wrapper->AppliedModifierDesc)
		{
			UE_LOG(LogTemp, Log, TEXT("[TestAttribute] Modifier %d: Source = %s, Effect = %s"), Index, *Desc->GetSourceString(),
				*Desc->GetEffectString());
			Index++;
		}
	}
}

void UCardDescObject::OnRep_FloatWrapperList()
{
}

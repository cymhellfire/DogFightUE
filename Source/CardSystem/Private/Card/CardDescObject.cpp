#include "Card/CardDescObject.h"
#include "Net/UnrealNetwork.h"
#include "AttributeSystem/Attribute/Attribute.h"
#include "AttributeSystem/Attribute/AttributeBase.h"
#include "UnrealIntegration/UObject/AttributeModifierDescObject.h"

void UCardDescObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(UCardDescObject, BooleanWrapperMap, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UCardDescObject, IntegerWrapperMap, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UCardDescObject, FloatWrapperMap, SharedParams);
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
			TSharedPtr<FAttributeBooleanWrapper> NewWrapper = MakeShareable(new FAttributeBooleanWrapper);
			NewWrapper->BaseValue = BooleanAttribute->GetRawValue();
			NewWrapper->Value = BooleanAttribute->GetValue();
			BooleanAttribute->OnValueChanged.AddLambda([this](TSharedPtr<FAttributeBase> Attribute)
			{
				if (auto ConvertAttr = StaticCastSharedPtr<FAttributeBoolean>(Attribute))
				{
					if (auto Wrapper = GetBooleanAttributeWrapperByName(Attribute->GetName()))
					{
						MARK_PROPERTY_DIRTY_FROM_NAME(UCardDescObject, BooleanWrapperMap, this);
						Wrapper->Value = ConvertAttr->GetValue();
					}
				}
			});

			MARK_PROPERTY_DIRTY_FROM_NAME(UCardDescObject, BooleanWrapperMap, this);
			BooleanWrapperMap.Add(AttributeName, NewWrapper);
		}
		break;
	case ADT_Integer:
		if (auto IntegerAttribute = StaticCastSharedPtr<FAttributeInteger>(InAttribute))
		{
			TSharedPtr<FAttributeIntegerWrapper> NewWrapper = MakeShareable(new FAttributeIntegerWrapper);
			NewWrapper->BaseValue = IntegerAttribute->GetRawValue();
			NewWrapper->Value = IntegerAttribute->GetValue();
			IntegerAttribute->OnValueChanged.AddLambda([this](TSharedPtr<FAttributeBase> Attribute)
			{
				if (auto ConvertAttr = StaticCastSharedPtr<FAttributeInteger>(Attribute))
				{
					if (auto Wrapper = GetIntegerAttributeWrapperByName(Attribute->GetName()))
					{
						MARK_PROPERTY_DIRTY_FROM_NAME(UCardDescObject, IntegerWrapperMap, this);
						Wrapper->Value = ConvertAttr->GetValue();
					}
				}
			});

			MARK_PROPERTY_DIRTY_FROM_NAME(UCardDescObject, IntegerWrapperMap, this);
			IntegerWrapperMap.Add(AttributeName, NewWrapper);
		}
		break;
	case ADT_Float:
		if (auto FloatAttribute = StaticCastSharedPtr<FAttributeFloat>(InAttribute))
		{
			TSharedPtr<FAttributeFloatWrapper> NewWrapper = MakeShareable(new FAttributeFloatWrapper);
			NewWrapper->BaseValue = FloatAttribute->GetRawValue();
			NewWrapper->Value = FloatAttribute->GetValue();
			FloatAttribute->OnValueChanged.AddLambda([this](TSharedPtr<FAttributeBase> Attribute)
			{
				if (auto ConvertAttr = StaticCastSharedPtr<FAttributeFloat>(Attribute))
				{
					if (auto Wrapper = GetFloatAttributeWrapperByName(Attribute->GetName()))
					{
						MARK_PROPERTY_DIRTY_FROM_NAME(UCardDescObject, FloatWrapperMap, this);
						Wrapper->Value = ConvertAttr->GetValue();
					}
				}
			});

			MARK_PROPERTY_DIRTY_FROM_NAME(UCardDescObject, FloatWrapperMap, this);
			FloatWrapperMap.Add(AttributeName, NewWrapper);
		}
		break;
	case ADT_None:
	default: ;
	}

	return true;
}

TSharedPtr<FAttributeBooleanWrapper> UCardDescObject::GetBooleanAttributeWrapperByName(FName InName)
{
	auto Wrapper = BooleanWrapperMap.Find(InName);
	return Wrapper ? *Wrapper : nullptr;
}

TSharedPtr<FAttributeIntegerWrapper> UCardDescObject::GetIntegerAttributeWrapperByName(FName InName)
{
	auto Wrapper = IntegerWrapperMap.Find(InName);
	return Wrapper ? *Wrapper : nullptr;
}

TSharedPtr<FAttributeFloatWrapper> UCardDescObject::GetFloatAttributeWrapperByName(FName InName)
{
	auto Wrapper = FloatWrapperMap.Find(InName);
	return Wrapper ? *Wrapper : nullptr;
}

void UCardDescObject::OnRep_IntegerWrapperMap()
{
	for (auto& Record : IntegerWrapperMap)
	{
		auto Wrapper = Record.Value;
		if (Wrapper->AppliedModifierDesc.Num() > 0)
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
}

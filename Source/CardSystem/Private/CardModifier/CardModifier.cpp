#include "CardModifier/CardModifier.h"

#include "CardSystem.h"
#include "AttributeSystem/AttributeFunctionLibrary.h"

void UCardModifier::Initialize()
{
	// Get modifier create argument from script
	auto CreateArgument = GetModifierCreateArgument();
	if (CreateArgument.DataType == ADT_None)
	{
		UE_LOG(LogCardSystem, Error, TEXT("[CardModifier] Invalid data type used to create modifier."));
		return;
	}

	// Create modifier
	Modifier = FAttributeFunctionLibrary::CreateAttributeModifier(CreateArgument);

	// Set factor value
	switch(CreateArgument.DataType)
	{
	case ADT_Boolean:
		if (auto ConvertedModifier = StaticCastSharedPtr<TAttributeModifierBase<bool>>(Modifier))
		{
			ConvertedModifier->SetFactor(GetInitialBooleanValue());
		}
		break;
	case ADT_Integer:
		if (auto ConvertedModifier = StaticCastSharedPtr<TAttributeModifierBase<int32>>(Modifier))
		{
			ConvertedModifier->SetFactor(GetInitialIntegerValue());
		}
		break;
	case ADT_Float:
		if (auto ConvertedModifier = StaticCastSharedPtr<TAttributeModifierBase<float>>(Modifier))
		{
			ConvertedModifier->SetFactor(GetInitialFloatValue());
		}
		break;
	case ADT_None:
	default: ;
	}
}

FModifierCreateArgument UCardModifier::GetModifierCreateArgument_Implementation()
{
	FModifierCreateArgument CreateArgument;
	CreateArgument.DataType = ADT_Boolean;
	CreateArgument.OperatorType = EModifierOperatorType::MOT_Equal;

	return CreateArgument;
}

bool UCardModifier::GetInitialBooleanValue_Implementation()
{
	return false;
}

int32 UCardModifier::GetInitialIntegerValue_Implementation()
{
	return 0;
}

float UCardModifier::GetInitialFloatValue_Implementation()
{
	return 0.f;
}

void UCardModifier::ApplyToAttribute(TSharedPtr<FAttributeBase> InAttribute)
{
	if (!InAttribute.IsValid())
	{
		return;
	}

	// Apply modifier
	InAttribute->AddModifier(Modifier);

	// Record modifier target
	ModifiedAttribute = InAttribute;
}

void UCardModifier::RemoveFromTarget()
{
	if (!ModifiedAttribute.IsValid())
	{
		return;
	}

	auto PinnedAttribute = ModifiedAttribute.Pin();
	PinnedAttribute->RemoveModifier(Modifier);
}

EAttributeDataType UCardModifier::GetDataType() const
{
	return Modifier.IsValid() ? Modifier->GetDataType() : ADT_None;
}

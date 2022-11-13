#include "CardModifier/CardModifier.h"

#include "CardSystem.h"
#include "AttributeSystem/AttributeFunctionLibrary.h"

void UCardModifier::PostInitProperties()
{
	UObject::PostInitProperties();

	// Invoke Initialize
	Initialize();
}

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
	CreateModifier(CreateArgument);
}

FModifierCreateArgument UCardModifier::GetModifierCreateArgument_Implementation()
{
	FModifierCreateArgument CreateArgument;
	CreateArgument.DataType = ADT_Boolean;
	CreateArgument.OperatorType = EModifierOperatorType::MOT_Equal;

	return CreateArgument;
}

EAttributeDataType UCardModifier::GetDataType() const
{
	return Modifier.IsValid() ? Modifier->GetDataType() : ADT_None;
}

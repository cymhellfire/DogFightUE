#include "Pawns/TestAttributeModifier.h"

#include "UnrealIntegration/UObject/AttributeModifierDescObject.h"

void UTestAttributeModifier::PostInitProperties()
{
	Super::PostInitProperties();

	// Create test modifier
	FModifierCreateArgument CreateArgument;
	CreateArgument.DataType = ADT_Integer;
	CreateArgument.OperatorType = EModifierOperatorType::MOT_Multiply;
	CreateArgument.InitIntegerValue = 2;

	CreateModifier(CreateArgument);
}

void UTestAttributeModifier::InitializeDescObject(UAttributeModifierDescObject* InDesc)
{
	InDesc->SetSourceString(GetName());
	InDesc->SetEffectString("Double Value");
}

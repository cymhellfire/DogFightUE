#include "UnrealIntegration/UObject/AttributeModifierBasedObject.h"

bool UAttributeModifierBasedObject::CreateModifier(const FModifierCreateArgument& InArgument)
{
	return IAttributeModifierCarrierInterface::CreateModifier(InArgument);
}

void UAttributeModifierBasedObject::RemoveFromTarget()
{
	IAttributeModifierCarrierInterface::RemoveFromTarget();
}

void UAttributeModifierBasedObject::OnModifierCreated(TSharedPtr<FAttributeModifierBase> InModifier)
{
	Modifier = InModifier;
}



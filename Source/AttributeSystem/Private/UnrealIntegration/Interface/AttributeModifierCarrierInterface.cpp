#include "UnrealIntegration/Interface/AttributeModifierCarrierInterface.h"

#include "AttributeSystem/AttributeFunctionLibrary.h"

bool IAttributeModifierCarrierInterface::CreateModifier(const FModifierCreateArgument& InArgument)
{
	auto NewModifier = FAttributeFunctionLibrary::CreateAttributeModifier(InArgument);
	if (NewModifier && NewModifier.IsValid())
	{
		// Record new modifier
		OnModifierCreated(NewModifier);
		return true;
	}

	return false;
}

void IAttributeModifierCarrierInterface::RemoveFromTarget(TSharedPtr<FAttributeBase>& OutTarget)
{
	auto Modifier = GetModifier();
	if (Modifier && Modifier.IsValid())
	{
		Modifier->RemoveFromTarget(OutTarget);
	}
}
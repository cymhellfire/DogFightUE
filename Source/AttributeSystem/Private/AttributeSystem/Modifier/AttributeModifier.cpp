#include "AttributeSystem/Modifier/AttributeModifier.h"

void FAttributeModifierBoolean::UpdateModifiedValue()
{
	// Directly override value
	ModifiedValue = ModifyFactor;

	bDirty = false;
}


// Dog Fight Game Code By CYM.


#include "GameObject/AttributeModifier/LuaAttributeModifierObject.h"

#include "AttributeSystem/Modifier/AttributeModifierBase.h"

FText ULuaAttributeModifierObject::GetEffectString() const
{
	if (Modifier.IsValid())
	{
		return Modifier->GetLocalizedEffectString();
	}

	return FText();
}

void ULuaAttributeModifierObject::InitializeDescObject(UAttributeModifierDescObject* InDesc)
{
	Super::InitializeDescObject(InDesc);

	// Let the lua script modify the description object
	InitDescObjectScript(InDesc);
}

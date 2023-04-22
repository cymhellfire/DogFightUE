// Dog Fight Game Code By CYM.


#include "GameObject/AttributeModifier/LuaAttributeModifierObject.h"

void ULuaAttributeModifierObject::InitModifier(const FString& InScript)
{
	if (InScript.IsEmpty())
	{
		return;
	}

	LoadAndInitModifierScript(InScript);
	ModifierScript = InScript;
}

void ULuaAttributeModifierObject::InitializeDescObject(UAttributeModifierDescObject* InDesc)
{
	Super::InitializeDescObject(InDesc);

	// Let the lua script modify the description object
	InitDescObjectScript(InDesc);
}

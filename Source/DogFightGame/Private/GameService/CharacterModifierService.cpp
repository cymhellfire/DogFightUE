// Dog Fight Game Code By CYM.


#include "GameService/CharacterModifierService.h"

UCharacterStatusModifier* UCharacterModifierService::CreateCharacterStatusModifier(int32 InId)
{
	return SpawnCharacterModifierInternal(InId);
}

#include "GameService/CardModifierService.h"

UCardAttributeModifier* UCardModifierService::CreateCardModifier(int32 InId)
{
	return CreateCardModifierInternal(InId);
}

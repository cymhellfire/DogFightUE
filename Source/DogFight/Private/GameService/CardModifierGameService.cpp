#include "GameService/CardModifierGameService.h"

#include "CardModifier/CardModifier.h"

UClass* UCardModifierGameService::GetDefaultModifierClass()
{
	return UCardModifier::StaticClass();
}

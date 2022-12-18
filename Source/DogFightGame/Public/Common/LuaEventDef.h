#pragma once

#include "LuaEventDef.generated.h"

UENUM(BlueprintType)
namespace ELuaEvent
{
	enum Type
	{
		LuaEvent_ReadyPlayerCount,
		LuaEvent_ReceiveTitleMessage,
	};
}

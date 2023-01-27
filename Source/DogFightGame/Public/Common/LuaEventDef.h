#pragma once

#include "LuaEventDef.generated.h"

UENUM(BlueprintType)
namespace ELuaEvent
{
	enum Type
	{
		LuaEvent_ReadyPlayerCount,
		LuaEvent_ReceiveTitleMessage,
		LuaEvent_GameTimelineChanged,
		// ------------------ Card --------------------
		LuaEvent_CardListChanged,
		/** Triggered when local player begin using a card (Client only) */
		LuaEvent_MyCardBeginUsing,
		/** Triggered when local player finished a card (Client only) */
		LuaEvent_MyCardFinished,
		/** Triggered when local player cancelled using card (Client only) */
		LuaEvent_MyCardCancelled,
		/** Triggered when any player begin using a card (Server only) */
		LuaEvent_PlayerCardBeginUsing,
		/** Triggered when any player finished a card (Server only) */
		LuaEvent_PlayerCardFinished,
		/** Triggered when any player cancelled using card (Server only) */
		LuaEvent_PlayerCardCancelled,
	};
}

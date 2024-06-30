#pragma once

#include "LuaEventDef.generated.h"

UENUM(BlueprintType)
namespace ELuaEvent
{
	enum Type
	{
		// ----------------- Generic -------------------
		LuaEvent_GameStartup,
		LuaEvent_PostLoadMap,
		LuaEvent_NetworkFailure,
		// --------------- Game Lobby ------------------
		LuaEvent_GameLobbyLevelLoaded,
		LuaEvent_OnPlayerEnterLobby,
		LuaEvent_OnPlayerLeaveLobby,
		LuaEvent_OnGameReadyChanged,
		LuaEvent_OnAIPlayerCountChanged,
		LuaEvent_OnGameplayExperienceChanged,
		LuaEvent_SessionDismiss,
		LuaEvent_NotifyGameLoading,

		
		LuaEvent_ReadyPlayerCount,
		LuaEvent_ReceiveTitleMessage,
		LuaEvent_GameTimelineChanged,
		LuaEvent_ReceiveInGameChatMessage,
		// --------------- Game Flow ------------------
		LuaEvent_StartPlayerRound,
		LuaEvent_FinishPlayerRound,
		LuaEvent_OnPlayerCharacterRevived,
		LuaEvent_OnPlayerCharacterDead,
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
		// ------------------ UI --------------------
		LuaEvent_ShowWidget,
		LuaEvent_CloseWidget,
	};
}

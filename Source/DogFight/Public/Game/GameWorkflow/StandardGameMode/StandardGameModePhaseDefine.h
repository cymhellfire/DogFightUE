// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"

namespace StandardGameModePhase
{
	extern DOGFIGHT_API const FName EnteringMap;			// Players are entering this map, actors are not yet ticking
	extern DOGFIGHT_API const FName WaitingForStart;		// Actors are ticking, but the match has not yet begun
	extern DOGFIGHT_API const FName SpawnPlayers;			// Spawn character for players in game.
	extern DOGFIGHT_API const FName FreeMoving;				// All players can move around without limitation.
	extern DOGFIGHT_API const FName DecideOrder;			// The order of players can take action will be decided here.
	extern DOGFIGHT_API const FName SelectAbility;			// Player select the start ability in this phase.
	extern DOGFIGHT_API const FName PlayerRoundBegin;		// Phase before a player's round begin.
	extern DOGFIGHT_API const FName PlayerRound;			// Specified player can take action in this phase.
	extern DOGFIGHT_API const FName CharacterReturn;		// Current player's character will return to the position where it's at round begin.
	extern DOGFIGHT_API const FName DiscardCards;			// Current player should discard all cards exceed the count limitation.
	extern DOGFIGHT_API const FName PlayerRoundEnd;			// Phase after a player's round end.
	extern DOGFIGHT_API const FName CheckGameEnd;			// Check if the game should end or continue.
	extern DOGFIGHT_API const FName GameSummary;			// Summary the game and display the result.
	extern DOGFIGHT_API const FName WaitingPostMatch;		// Match has ended but actors are still ticking.

	extern DOGFIGHT_API const FName WaitForRagdoll;			// Wait for all character in ragdoll recovered.
	extern DOGFIGHT_API const FName RequestResponseCard;	// Request given type card from target player.
}

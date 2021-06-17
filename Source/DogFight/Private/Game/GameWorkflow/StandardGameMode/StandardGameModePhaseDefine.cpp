#include "Game/GameWorkflow/StandardGameMode/StandardGameModePhaseDefine.h"

namespace StandardGameModePhase
{
	// Process phases
	const FName EnteringMap = FName(TEXT("EnteringMap"));
	const FName WaitingForStart = FName(TEXT("WaitingForStart"));
	const FName SpawnPlayers = FName(TEXT("SpawnPlayers"));
	const FName FreeMoving = FName(TEXT("FreeMoving"));
	const FName DecideOrder = FName(TEXT("DecideOrder"));
	const FName SelectAbility = FName(TEXT("SelectAbility"));
	const FName PlayerRoundBegin = FName(TEXT("PlayerRoundBegin"));
	const FName PlayerRound = FName(TEXT("PlayerRound"));
	const FName CharacterReturn = FName(TEXT("CharacterReturn"));
	const FName DiscardCards = FName(TEXT("DiscardCards"));
	const FName PlayerRoundEnd = FName(TEXT("PlayerRoundEnd"));
	const FName CheckGameEnd = FName(TEXT("CheckGameEnd"));
	const FName GameSummary = FName(TEXT("GameSummary"));
	const FName WaitingPostMatch = FName(TEXT("WaitingPostMatch"));

	// Floating phases
	const FName WaitForRagdoll = FName(TEXT("WaitForRagdoll"));
	const FName RequestResponseCard = FName(TEXT("RequestResponseCard"));
}
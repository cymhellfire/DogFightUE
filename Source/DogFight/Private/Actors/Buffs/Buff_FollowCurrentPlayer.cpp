// Dog Fight Game Code By CYM.


#include "Actors/Buffs/Buff_FollowCurrentPlayer.h"


#include "Actors/Managers/BuffQueue.h"
#include "Game/GameRoundsTimeline.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardGameState.h"
#include "Game/StandardPlayerState.h"
#include "Pawns/StandardModePlayerCharacter.h"
#include "Actors/Vfx/VfxBase.h"

void ABuff_FollowCurrentPlayer::OnTargetPlayerRoundEnd()
{
	Super::OnTargetPlayerRoundEnd();

	if (!bPendingEnd)
	{
		MoveToNextPlayer();
	}
}

void ABuff_FollowCurrentPlayer::MoveToNextPlayer()
{
	if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (AStandardGameState* StandardGameState = StandardGameMode->GetGameState<AStandardGameState>())
		{
			if (AGameRoundsTimeline* Timeline = StandardGameState->GetGameRoundsTimeline())
			{
				const int32 NextPlayerId = Timeline->GetNextPlayerId();
				if (AStandardModePlayerCharacter* NextPlayerCharacter = StandardGameMode->GetPlayerCharacterById(NextPlayerId))
				{
					// Unregister from current actor
					UnregisterCallbackFromCharacter();
					OnDetachFromCurrentPlayer(Timeline->GetCurrentPlayerId());

					// Register to new actor
					TargetActor = NextPlayerCharacter;
					RegisterCallbackToCharacter(NextPlayerCharacter);
					OnAttachToNextPlayer(NextPlayerId);

					UE_LOG(LogDogFight, Log, TEXT("Buff [%s] moves from Player[%d] to Player[%d]"), *GetName(), Timeline->GetCurrentPlayerId(), NextPlayerId);

					// Transfer vfx to next actor
					if (IsValid(VfxActor))
					{
						VfxActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
						VfxActor->SetTargetActor(NextPlayerCharacter);
					}
				}
			}
		}
	}
}

void ABuff_FollowCurrentPlayer::OnDetachFromCurrentPlayer(int32 PlayerId)
{
	if (AStandardModePlayerCharacter* PlayerCharacter = Cast<AStandardModePlayerCharacter>(TargetActor))
	{
		PlayerCharacter->MulticastAddFloatingText(GetBuffEndText());
	}
}

void ABuff_FollowCurrentPlayer::OnAttachToNextPlayer(int32 PlayerId)
{
	if (AStandardModePlayerCharacter* PlayerCharacter = Cast<AStandardModePlayerCharacter>(TargetActor))
	{
		PlayerCharacter->MulticastAddFloatingText(GetBuffStartText());
	}
}

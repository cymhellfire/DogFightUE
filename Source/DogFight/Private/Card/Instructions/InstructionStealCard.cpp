// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionStealCard.h"

#include "AI/StandardModeAIController.h"
#include "Pawns/StandardModePlayerCharacter.h"
#include "Card/CardBase.h"
#include "Actors/Vfx/StealCardBeamVfx.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardPlayerState.h"
#include "Player/StandardModePlayerController.h"

bool UInstructionStealCard::HandleActorTarget(AActor* Target)
{
	const bool Result = Super::HandleActorTarget(Target);

	if (Result && IsValid(Target))
	{
		AStandardPlayerState* SourcePlayerState = nullptr;
		AStandardPlayerState* DestPlayerState = nullptr;
		if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(Target))
		{
			AController* SupremeController = StandardModePlayerCharacter->GetSupremeController();
			if (SupremeController)
			{
				SourcePlayerState = SupremeController->GetPlayerState<AStandardPlayerState>();
			}
		}
		AController* DestController = OwnerCard->GetOwnerPlayerController();
		if (DestController)
		{
			DestPlayerState = DestController->GetPlayerState<AStandardPlayerState>();
		}

		if (!IsValid(SourcePlayerState) || !IsValid(DestPlayerState))
		{
			return false;
		}

		FTransferCardInfo TransferCardInfo;
		TransferCardInfo.TransferType = TCT_Random;
		TransferCardInfo.TransferCardData = { CardCount.GetValue() };

		if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
		{
			const int32 TransferCardCount = StandardGameMode->TransferCardsBetweenPlayer(SourcePlayerState, DestPlayerState, TransferCardInfo);

			if (TransferCardCount == 0)
			{
				const FText SrcPlayerName = FText::FromString(FString::Printf(TEXT("<PlayerName>%s</>"), *SourcePlayerState->GetPlayerName()));
				StandardGameMode->SendGameMessageToPlayer(
					FGameMessage{TEXT("System"), EGameMessageType::GMT_System, TEXT("GameMsg_NoCard"), {SrcPlayerName}},
					DestPlayerState->GetPlayerId());
			}
		}

		// Spawn vfx
		if (IsValid(BeamVfxClass))
		{
			AController* Controller = OwnerCard->GetOwnerPlayerController();
			AStealCardBeamVfx* NewBeamVfx = GetWorld()->SpawnActor<AStealCardBeamVfx>(BeamVfxClass);
			if (AStandardModePlayerController* PlayerController = Cast<AStandardModePlayerController>(Controller))
			{
				NewBeamVfx->SetTargetActor(PlayerController->GetActualPawn());
			}
			else if (AStandardModeAIController* AIController = Cast<AStandardModeAIController>(Controller))
			{
				NewBeamVfx->SetTargetActor(AIController->GetActualPawn());
			}
			else
			{
				UE_LOG(LogDogFight, Error, TEXT("Invalid card owner."));
				NewBeamVfx->ConditionalBeginDestroy();
			}

			NewBeamVfx->SetBeamTargetActor(Target);
		}
	}

	return false;
}

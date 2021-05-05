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
		AController* DestController = OwnerCard->GetOwnerPlayerController();
		if (DestController)
		{
			DestPlayerState = DestController->GetPlayerState<AStandardPlayerState>();
		}
		if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(Target))
		{
			AController* SupremeController = StandardModePlayerCharacter->GetSupremeController();
			if (SupremeController)
			{
				if (SupremeController == DestController)
				{
					// No need to steal card from self
					return true;
				}
				SourcePlayerState = SupremeController->GetPlayerState<AStandardPlayerState>();
			}
		}

		if (!IsValid(SourcePlayerState) || !IsValid(DestPlayerState))
		{
			return false;
		}

		FTransferCardInfo TransferCardInfo;
		TransferCardInfo.TransferType = TCT_Random;
		TransferCardInfo.TransferCardData = { CardCount.GetValue() };

		int32 TransferCardCount = 0;
		if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
		{
			TransferCardCount = StandardGameMode->TransferCardsBetweenPlayer(SourcePlayerState, DestPlayerState, TransferCardInfo);

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
			APawn* CardUserPawn = nullptr;
			if (AStandardModePlayerController* PlayerController = Cast<AStandardModePlayerController>(Controller))
			{
				CardUserPawn = PlayerController->GetActualPawn();
			}
			else if (AStandardModeAIController* AIController = Cast<AStandardModeAIController>(Controller))
			{
				CardUserPawn = AIController->GetActualPawn();
			}
			else
			{
				UE_LOG(LogDogFight, Error, TEXT("Invalid card owner."));
				return false;
			}

			AStealCardBeamVfx* NewBeamVfx = GetWorld()->SpawnActorDeferred<AStealCardBeamVfx>(BeamVfxClass, CardUserPawn->GetActorTransform(), Controller);
			NewBeamVfx->SetStealCardData(TransferCardCount, FColor::White);
			NewBeamVfx->FinishSpawning(CardUserPawn->GetActorTransform());

			NewBeamVfx->SetTargetActor(CardUserPawn);
			NewBeamVfx->SetBeamTargetActor(Target);
		}
	}

	return false;
}

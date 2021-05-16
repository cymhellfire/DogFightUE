// Dog Fight Game Code By CYM.


#include "Card/Instructions/HandleTargetInstructionBase.h"

#include "Card/GameCardTypes.h"
#include "Card/CardBase.h"
#include "Actors/Interfaces/GameCardUserPlayerControllerInterface.h"
#include "Common/Localization.h"
#include "Game/StandardGameMode.h"
#include "GameFramework/PlayerState.h"

UHandleTargetInstructionBase::UHandleTargetInstructionBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoFinish = true;
	bSkipOneBroadcast = false;
}

bool UHandleTargetInstructionBase::ProcessTarget()
{
	if (OwnerCard == nullptr)
	{
		UE_LOG(LogGameCards, Error, TEXT("No OwnerCard assigned to this instruction."));
		return true;
	}

	FVector CameraFocusPos;

	if (CurrentTargetIndex < OwnerCard->GetTargetInfoCount())
	{
		FCardInstructionTargetInfo TargetInfo = OwnerCard->GetTargetInfo(CurrentTargetIndex);
		AActor* ActorPtr = nullptr;
		FVector Position, Direction;
		int32 Type = ParseTargetInfo(TargetInfo, &ActorPtr, Position, Direction);
		switch (Type)
		{
		case 1:
			HandleActorTarget(ActorPtr);
			CameraFocusPos = ActorPtr->GetActorLocation();
			break;
		case 2:
			HandlePositionTarget(Position);
			CameraFocusPos = Position;
			break;
		case 3:
			HandleDirectionTarget(Direction);
			CameraFocusPos = GetOwnerControlledPawn()->GetActorLocation();
			break;
		default:
			UE_LOG(LogGameCards, Error, TEXT("Parsed an invalid target information for %s"), *OwnerCard->GetName());
			break;;
		}
		CurrentTargetIndex++;
	}

	// Broadcast camera focus event
	if (CameraFocusType == EInstructionCameraFocusType::ICFT_Target)
	{
		if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
		{
			if (bForciblyCameraEventToOwner)
			{
				StandardGameMode->BroadcastCameraFocusEvent(
				FCameraFocusEvent{
					OwnerCard->GetOwnerPlayerController()->GetPlayerState<APlayerState>()->GetPlayerId(),
					CameraFocusPos.X,
					CameraFocusPos.Y,
					ECameraFocusEventType::Type::OwnerForced
				});
			}
			else
			{
				StandardGameMode->BroadcastCameraFocusEvent(
					FCameraFocusEvent{
						-1,
						CameraFocusPos.X,
						CameraFocusPos.Y,
						ECameraFocusEventType::Type::Default
					});
			}
		}
	}

	return CurrentTargetIndex >= OwnerCard->GetTargetInfoCount();
}

void UHandleTargetInstructionBase::Execute()
{
	Super::Execute();

	while(!ProcessTarget()) { }

	OnHandledAllTarget();

	if (bAutoFinish)
	{
		Finish();
	}
}

int32 UHandleTargetInstructionBase::ParseTargetInfo(FCardInstructionTargetInfo TargetInfo, AActor** ActorPtr, FVector& PositionValue, FVector& DirectionValue)
{
	switch(TargetInfo.TargetType)
	{
	case ECardInstructionTargetType::Actor:
		*ActorPtr = TargetInfo.ActorPtr;
		return 1;
	case ECardInstructionTargetType::Position:
		PositionValue = TargetInfo.PositionValue;
		return 2;
	case ECardInstructionTargetType::Direction:
		DirectionValue = TargetInfo.DirectionValue;
		return 3;
	default: /* Do nothing with default branch. */ ;
	}
	
	return 0;
}

bool UHandleTargetInstructionBase::HandleActorTarget(AActor* Target)
{
	if (bSkipOneBroadcast)
	{
		bSkipOneBroadcast = false;
		return true;
	}

	// Broadcast game message
	if (IGameCardUserPlayerControllerInterface* UserPlayerController = Cast<IGameCardUserPlayerControllerInterface>(OwnerCard->GetOwnerPlayerController()))
	{
		// Check if it's self-selected
		if (GetOwnerControlledPawn() == Target)
		{
			UserPlayerController->BroadcastCardTargetingResult(OwnerCard->GetCardDisplayInfo().GetCardNameText(),
				FText::FromStringTable(ST_INGAME_UI_LOC, TEXT("GameMsg_Self")), ECardInstructionTargetType::Actor);
		}
		else
		{
			FString TargetName = Target->GetName();

			// Get owner player name if possible
			if (APawn* TargetPawn = Cast<APawn>(Target))
			{
				if (APlayerState* PlayerState = TargetPawn->GetPlayerState())
				{
					TargetName = FString::Printf(TEXT("<PlayerName>%s</>"), *PlayerState->GetPlayerName());
				}
			}

			UserPlayerController->BroadcastCardTargetingResult(OwnerCard->GetCardDisplayInfo().GetCardNameText(),
				FText::FromString(TargetName), ECardInstructionTargetType::Actor);
		}
	}

	return true;
}

void UHandleTargetInstructionBase::HandlePositionTarget(FVector Position)
{
	if (bSkipOneBroadcast)
	{
		bSkipOneBroadcast = false;
		return;
	}

	// Broadcast game message
	if (IGameCardUserPlayerControllerInterface* UserPlayerController = Cast<IGameCardUserPlayerControllerInterface>(OwnerCard->GetOwnerPlayerController()))
	{
		UserPlayerController->BroadcastCardTargetingResult(OwnerCard->GetCardDisplayInfo().GetCardNameText(),
			FText::FromString(Position.ToString()), ECardInstructionTargetType::Position);
	}
}

void UHandleTargetInstructionBase::HandleDirectionTarget(FVector Direction)
{
	if (bSkipOneBroadcast)
	{
		bSkipOneBroadcast = false;
		return;
	}

	// Broadcast game message
	if (IGameCardUserPlayerControllerInterface* UserPlayerController = Cast<IGameCardUserPlayerControllerInterface>(OwnerCard->GetOwnerPlayerController()))
	{
		UserPlayerController->BroadcastCardTargetingResult(OwnerCard->GetCardDisplayInfo().GetCardNameText(),
			FText::FromString(Direction.ToString()), ECardInstructionTargetType::Direction);
	}
}


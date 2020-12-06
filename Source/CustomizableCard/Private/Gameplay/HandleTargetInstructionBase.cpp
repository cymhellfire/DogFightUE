// Dog Fight Game Code By CYM.


#include "HandleTargetInstructionBase.h"

#include "CustomizableCard.h"
#include "CardBase.h"
#include "GameCardUserPlayerControllerInterface.h"

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
		UE_LOG(LogCustomizableCard, Error, TEXT("No OwnerCard assigned to this instruction."));
		return true;
	}

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
			break;
		case 2:
			HandlePositionTarget(Position);
			break;
		case 3:
			HandleDirectionTarget(Direction);
			break;
		default:
			UE_LOG(LogCustomizableCard, Error, TEXT("Parsed an invalid target information for %s"), *OwnerCard->GetName());
			break;;
		}
		CurrentTargetIndex++;
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

void UHandleTargetInstructionBase::HandleActorTarget(AActor* Target)
{
	if (bSkipOneBroadcast)
	{
		bSkipOneBroadcast = false;
		return;
	}

	// Broadcast game message
	if (IGameCardUserPlayerControllerInterface* UserPlayerController = Cast<IGameCardUserPlayerControllerInterface>(OwnerCard->GetOwnerPlayerController()))
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


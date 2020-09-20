// Dog Fight Game Code By CYM.


#include "HandleTargetInstructionBase.h"

#include "CustomizableCard.h"
#include "CardBase.h"

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

	Finish();
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


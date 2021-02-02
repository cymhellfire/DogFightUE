// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionSelectSelf.h"


#include "Card/CardBase.h"
#include "Actors/Interfaces/GameCardUserPlayerControllerInterface.h"
#include "Actors/Interfaces/GameTargetProviderInterface.h"

void UInstructionSelectSelf::Execute()
{
	FCardInstructionTargetInfo NewTargetInfo;
	if (IGameCardUserPlayerControllerInterface* UserPlayerController = Cast<IGameCardUserPlayerControllerInterface>(GetOwnerCard()->GetOwnerPlayerController()))
	{
		NewTargetInfo.ActorPtr = UserPlayerController->GetActualPawn();
	}
	NewTargetInfo.TargetType = ECardInstructionTargetType::Actor;

	if (IGameTargetProviderInterface* Provider = Cast<IGameTargetProviderInterface>(GetOwnerCard()->GetOwnerPlayerController()))
	{
		GetOwnerCard()->PushTargetInfo(NewTargetInfo);
	}

	Finish();
}

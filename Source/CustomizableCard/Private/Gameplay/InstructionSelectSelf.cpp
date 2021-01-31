// Dog Fight Game Code By CYM.


#include "Gameplay/InstructionSelectSelf.h"


#include "Gameplay/CardBase.h"
#include "Interfaces/GameCardUserPlayerControllerInterface.h"
#include "Interfaces/GameTargetProviderInterface.h"

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

// Dog Fight Game Code By CYM.


#include "DogFightGameModeBase.h"

#include "DogFightPlayerController.h"

void ADogFightGameModeBase::RequestFinishAndExitToMainMenu()
{
	ADogFightPlayerController* LocalPlayerController = nullptr;

	// Notify everyone else to leave game
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (ADogFightPlayerController* DogFightPlayerController = Cast<ADogFightPlayerController>(*Iterator))
		{
			if (!DogFightPlayerController->IsLocalController())
			{
				DogFightPlayerController->RpcReturnToMainMenuWithReason(EReturnToMainMenuReason::HostLeft);
			}
			else
			{
				LocalPlayerController = DogFightPlayerController;
			}
		}
	}

	if (LocalPlayerController != nullptr)
	{
		LocalPlayerController->HandleReturnToMainMenu();
	}
}

void ADogFightGameModeBase::NotifyClientGameWillStart()
{
	// Tell every client game will start
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (ADogFightPlayerController* DogFightPlayerController = Cast<ADogFightPlayerController>(*Iterator))
		{
			DogFightPlayerController->RpcPreStartGame();
		}
	}
}

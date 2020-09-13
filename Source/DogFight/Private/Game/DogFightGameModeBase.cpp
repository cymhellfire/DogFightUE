// Dog Fight Game Code By CYM.


#include "DogFightGameModeBase.h"

#include "DogFightPlayerController.h"

void ADogFightGameModeBase::RequestFinishAndExitToMainMenu()
{
	ADogFightPlayerController* LocalPlayerController = nullptr;

	// Notify everyone else to leave game
	for (ADogFightPlayerController* PlayerController : PlayerControllerList)
	{
		if (!PlayerController->IsLocalController())
		{
			PlayerController->RpcReturnToMainMenuWithReason(EReturnToMainMenuReason::HostLeft);
		}
		else
		{
			LocalPlayerController = PlayerController;
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
	for (ADogFightPlayerController* PlayerController : PlayerControllerList)
	{
		PlayerController->RpcPreStartGame();
	}
}

void ADogFightGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ADogFightPlayerController* DogFightPlayerController = Cast<ADogFightPlayerController>(NewPlayer))
	{
		if (!PlayerControllerList.Contains(DogFightPlayerController))
		{
			PlayerControllerList.Add(DogFightPlayerController);
		}

		// Request Player Info
		DogFightPlayerController->RpcHostUploadPlayerInfo();
	}
}

void ADogFightGameModeBase::Logout(AController* Exiting)
{
	if (ADogFightPlayerController* DogFightPlayerController = Cast<ADogFightPlayerController>(Exiting))
	{
		if (PlayerControllerList.Contains(DogFightPlayerController))
		{
			PlayerControllerList.Remove(DogFightPlayerController);
		}
	}

	Super::Logout(Exiting);
}

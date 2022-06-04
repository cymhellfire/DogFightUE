// Dog Fight Game Code By CYM.


#include "GameMode/DogFightGameStateBase.h"

#include "GameMode/DogFightGameModeBase.h"
#include "Player/DogFightPlayerController.h"

void ADogFightGameStateBase::FinishGameAndReturnToMainMenu()
{
	if (AuthorityGameMode)
	{
		// Invoke the GameMode corresponding method if this is server
		ADogFightGameModeBase* GameMode = Cast<ADogFightGameModeBase>(AuthorityGameMode);
		if (GameMode != nullptr)
		{
			GameMode->RequestFinishAndExitToMainMenu();
		}
	}
	else
	{
		// Only do self job on client side
		ADogFightPlayerController* const PlayerController = Cast<ADogFightPlayerController>(GetGameInstance()->GetFirstLocalPlayerController());
		if (PlayerController != nullptr)
		{
			PlayerController->HandleReturnToMainMenu();
		}
	}
}

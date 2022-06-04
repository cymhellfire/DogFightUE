// Dog Fight Game Code By CYM.


#include "GameMode/DogFightGameModeBase.h"

#include "Player/DogFightPlayerController.h"

void ADogFightGameModeBase::RequestFinishAndExitToMainMenu()
{
	ADogFightPlayerController* LocalPlayerController = nullptr;

	// Notify everyone else to leave game
	for (ADogFightPlayerController* PlayerController : PlayerControllerList)
	{
		if (!PlayerController->IsLocalController())
		{
			PlayerController->ClientReturnToMainMenuWithReason(EReturnToMainMenuReason::HostLeft);
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
		PlayerController->ClientPreStartGame();
	}
}

float ADogFightGameModeBase::CalculateDamage(AActor* DamageTaker, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return Damage;
}

EPlayerRelation ADogFightGameModeBase::GetPlayersRelation(AController* PlayerA, AController* PlayerB)
{
	return PlayerA == PlayerB ? EPlayerRelation::PR_Ally : EPlayerRelation::PR_Enemy;
}

void ADogFightGameModeBase::BeginPlay()
{
	Super::BeginPlay();
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
		DogFightPlayerController->ClientHostUploadPlayerInfo();
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

// Dog Fight Game Code By CYM.


#include "DogFightGameModeBase.h"

#include "DogFightPlayerController.h"
#include "DamageCalculatorBase.h"

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

float ADogFightGameModeBase::CalculateDamage(AActor* DamageTaker, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageCalculator != nullptr)
	{
		return DamageCalculator->CalculateActualDamage(DamageTaker, Damage, DamageEvent, EventInstigator, DamageCauser);
	}

	return Damage;
}

void ADogFightGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// Create damage calculator
	if (IsValid(DamageCalculatorClass))
	{
		DamageCalculator = NewObject<UDamageCalculatorBase>(this, DamageCalculatorClass, FName(TEXT("DamageCalculator")));
	}
}

void ADogFightGameModeBase::BeginDestroy()
{
	Super::BeginDestroy();

	// Destroy damage calculator
	if (DamageCalculator != nullptr)
	{
		DamageCalculator->ConditionalBeginDestroy();
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

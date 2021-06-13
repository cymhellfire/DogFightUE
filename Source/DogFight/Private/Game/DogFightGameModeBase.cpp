// Dog Fight Game Code By CYM.


#include "Game/DogFightGameModeBase.h"

#include "Player/DogFightPlayerController.h"
#include "Game/DamageCalculatorBase.h"
#include "Actors/Managers/ShieldManager.h"

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
	if (DamageCalculator != nullptr)
	{
		return DamageCalculator->CalculateActualDamage(DamageTaker, Damage, DamageEvent, EventInstigator, DamageCauser);
	}

	return Damage;
}

EPlayerRelation ADogFightGameModeBase::GetPlayersRelation(AController* PlayerA, AController* PlayerB)
{
	return PlayerA == PlayerB ? EPlayerRelation::PR_Ally : EPlayerRelation::PR_Enemy;
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

void ADogFightGameModeBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UWorld* MyWorld = GetWorld();
	if (IsValid(MyWorld) && !MyWorld->IsPreviewWorld())
	{
		// Create phase state machine
		InitializeStateMachine();
	}
}

void ADogFightGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (UWorld* MyWorld = GetWorld())
	{
		// Create Shield Manager
		ShieldManager = MyWorld->SpawnActor<AShieldManager>(AShieldManager::StaticClass());
		if (ShieldManager)
		{
			ShieldManager->Rename(TEXT("ShieldManager"));
		}
	}
}

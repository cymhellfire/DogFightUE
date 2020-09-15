// Dog Fight Game Code By CYM.


#include "StandardGameMode.h"

#include "DogFight.h"
#include "DogFightGameInstance.h"
#include "StandardGameState.h"
#include "StandardHUD.h"
#include "StandardModePlayerController.h"
#include "StandardModePlayerPawn.h"
#include "StandardPlayerState.h"

namespace GamePhase
{
	const FName EnteringMap = FName(TEXT("EnteringMap"));
	const FName WaitingForStart = FName(TEXT("WaitingForStart"));
	const FName InProgress = FName(TEXT("InProgress"));
	const FName WaitingPostMatch = FName(TEXT("WaitingPostMatch"));
}

AStandardGameMode::AStandardGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = AStandardModePlayerPawn::StaticClass();
	HUDClass = AStandardHUD::StaticClass();
	PlayerControllerClass = AStandardModePlayerController::StaticClass();
	GameStateClass = AStandardGameState::StaticClass();
	PlayerStateClass = AStandardPlayerState::StaticClass();

	CurrentGamePhase = GamePhase::EnteringMap;
	GameStartDelay = 5;
}

void AStandardGameMode::EnablePlayerClickMovement()
{
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStandardModePlayerController::StaticClass(), ActorList);

	for(AActor* Actor : ActorList)
	{
		AStandardModePlayerController* PlayerController = Cast<AStandardModePlayerController>(Actor);
		if (PlayerController != nullptr)
		{
			PlayerController->SetClickMovementEnabled(true);
		}
	}
}

void AStandardGameMode::DisablePlayerClickMovement()
{
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStandardModePlayerController::StaticClass(), ActorList);

	for(AActor* Actor : ActorList)
	{
		AStandardModePlayerController* PlayerController = Cast<AStandardModePlayerController>(Actor);
		if (PlayerController != nullptr)
		{
			PlayerController->SetClickMovementEnabled(false);
		}
	}
}

void AStandardGameMode::Logout(AController* Exiting)
{
	// Check if this Controller is recorded
	AStandardModePlayerController* StandardModePlayerController = Cast<AStandardModePlayerController>(Exiting);
	if (StandardModePlayerController != nullptr)
	{
		if (StandardPlayerControllerList.Contains(StandardModePlayerController))
		{
			StandardPlayerControllerList.Remove(StandardModePlayerController);
			
			UE_LOG(LogDogFight, Log, TEXT("Remove controller [%s] from list."), *StandardModePlayerController->GetName());
		}
	}

	// Continue to do other things
	Super::Logout(Exiting);
}

void AStandardGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Cast and record AStandardModePlayerController
	AStandardModePlayerController* StandardModePlayerController = Cast<AStandardModePlayerController>(NewPlayer);
	if (StandardModePlayerController != nullptr)
	{
		if (StandardPlayerControllerList.Contains(StandardModePlayerController) == false)
		{
			StandardPlayerControllerList.Add(StandardModePlayerController);

			UE_LOG(LogDogFight, Log, TEXT("Add controller [%s] to list."), *StandardModePlayerController->GetName());
		}
	}
}

void AStandardGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	GetWorldTimerManager().SetTimer(DefaultTimerHandle, this, &AStandardGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);

	// Set the joined player count to 1 since host is the first player
	PlayerJoinedGame = 1;
	OnJoinedPlayerCountChanged();
}

void AStandardGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// Set initial game state to 'EnteringMap'
	SetGamePhase(GamePhase::EnteringMap);
}

void AStandardGameMode::PlayerReadyForGame()
{
	PlayerJoinedGame++;
	OnJoinedPlayerCountChanged();
}

void AStandardGameMode::StartGame()
{
	SetGamePhase(GamePhase::InProgress);

	// Notify all player controller game has began
	for (AStandardModePlayerController* PlayerController : StandardPlayerControllerList)
	{
		PlayerController->GameStart();
	}
}

void AStandardGameMode::BeginPlay()
{
	// Not call super here, thus the BeginPlay event won't fire in Blueprint

	// Check if all players are already loaded map
	if (UDogFightGameInstance* GameInstance = Cast<UDogFightGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (GameInstance->GamePlayerCount == PlayerJoinedGame)
		{
			SetGamePhase(GamePhase::WaitingForStart);
		}
	}
}

void AStandardGameMode::DefaultTimer()
{
	static uint32 DebugTimerCounter = 0;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, FString::Printf(TEXT("Default timer tick: %d"), DebugTimerCounter));
	DebugTimerCounter++;

	AStandardGameState* StandardGameState = Cast<AStandardGameState>(GameState);
	if (StandardGameState && StandardGameState->GetRemainingTime() > 0)
	{
		StandardGameState->SetRemainingTime(StandardGameState->GetRemainingTime() - 1);

		if (StandardGameState->GetRemainingTime() <= 0)
		{
			if (CurrentGamePhase == GamePhase::WaitingForStart)
			{
				StartGame();
			}
		}
	}
}

void AStandardGameMode::SetGamePhase(FName NewPhase)
{
	if (CurrentGamePhase == NewPhase)
	{
		return;
	}

	UE_LOG(LogDogFight, Log, TEXT("GameMode: Changed to phase [%s]"), *NewPhase.ToString());

	CurrentGamePhase = NewPhase;

	OnGamePhaseChanged();

	// Sync game state
	if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
	{
		StandardGameState->SetCurrentGamePhase(CurrentGamePhase);
	}
}

void AStandardGameMode::OnGamePhaseChanged()
{
	if (CurrentGamePhase == GamePhase::WaitingForStart)
	{
		HandlePhaseWaitingForStart();
	}
	else if (CurrentGamePhase == GamePhase::InProgress)
	{
		HandlePhaseInProgress();
	}
	else if (CurrentGamePhase == GamePhase::WaitingPostMatch)
	{
		HandlePhaseWaitingPostMatch();
	}
}

void AStandardGameMode::HandlePhaseWaitingForStart()
{
	AStandardGameState* StandardGameState = Cast<AStandardGameState>(GameState);
	if (StandardGameState != nullptr && StandardGameState->GetRemainingTime() == 0)
	{
		if (GameStartDelay > 0)
		{
			StandardGameState->SetRemainingTime(GameStartDelay);
		}
	}
}

void AStandardGameMode::HandlePhaseInProgress()
{
}

void AStandardGameMode::HandlePhaseWaitingPostMatch()
{
}

void AStandardGameMode::OnJoinedPlayerCountChanged()
{
	if (UDogFightGameInstance* GameInstance = Cast<UDogFightGameInstance>(GetGameInstance()))
	{
		// Check if all players are loaded this map
		if (GameInstance->GamePlayerCount == PlayerJoinedGame)
		{
			SetGamePhase(GamePhase::WaitingForStart);
		}
		else
		{
			// Update the countdown content string
			AStandardGameState* StandardGameState = Cast<AStandardGameState>(GameState);
			if (StandardGameState != nullptr)
			{
				StandardGameState->SetCountdownContentString(FString::Printf(TEXT("%d/%d"), PlayerJoinedGame, GameInstance->GamePlayerCount));
			}
		}
	}
}

void AStandardGameMode::SetAllPlayerClickMove(bool bEnable)
{
	if (bEnable)
	{
		EnablePlayerClickMovement();
	}
	else
	{
		DisablePlayerClickMovement();
	}
}

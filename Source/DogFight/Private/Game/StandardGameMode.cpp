// Dog Fight Game Code By CYM.


#include "StandardGameMode.h"


#include "CardBase.h"
#include "DogFight.h"
#include "DogFightGameInstance.h"
#include "StandardGameState.h"
#include "StandardHUD.h"
#include "StandardModePlayerController.h"
#include "StandardModePlayerPawn.h"
#include "StandardPlayerState.h"
#include "GameplayCardPool.h"
#include "GameRoundsTimeline.h"

namespace GamePhase
{
	const FName EnteringMap = FName(TEXT("EnteringMap"));
	const FName WaitingForStart = FName(TEXT("WaitingForStart"));
	const FName SpawnPlayers = FName(TEXT("SpawnPlayers"));
	const FName FreeMoving = FName(TEXT("FreeMoving"));
	const FName DecideOrder = FName(TEXT("DecideOrder"));
	const FName PlayerRoundBegin = FName(TEXT("PlayerRoundBegin"));
	const FName PlayerRound = FName(TEXT("PlayerRound"));
	const FName PlayerRoundEnd = FName(TEXT("PlayerRoundEnd"));
	const FName CheckGameEnd = FName(TEXT("CheckGameEnd"));
	const FName GameSummary = FName(TEXT("GameSummary"));
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
	SpawnPlayerInterval = 0.5f;
	FreeMovingDuration = 5;

	// Enable the primary tick
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AStandardGameMode::EnablePlayerClickMovement()
{
	for (AStandardModePlayerController* PlayerController : StandardPlayerControllerList)
	{
		PlayerController->RpcSetClickMovementEnabled(true);
	}
}

void AStandardGameMode::DisablePlayerClickMovement()
{
	for (AStandardModePlayerController* PlayerController : StandardPlayerControllerList)
	{
		PlayerController->RpcSetClickMovementEnabled(false);
		// Also stop current movement
		PlayerController->StopCharacterMovementImmediately();
	}
}

void AStandardGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (DelayActionQueue.Num() > 0)
	{
		for(EGameModeDelayAction DelayAction : DelayActionQueue)
		{
			HandleDelayAction(DelayAction);
		}
		DelayActionQueue.Empty();
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

			StandardModePlayerController->OnPlayerDead.AddDynamic(this, &AStandardGameMode::OnPlayerDeadCallback);
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

	// Spawn Timeline actor
	if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
	{
		AGameRoundsTimeline* Timeline = GetWorld()->SpawnActor<AGameRoundsTimeline>(AGameRoundsTimeline::StaticClass(), FVector::ZeroVector,
			FRotator(0.f, 0.f,0.f));
		Timeline->SetOwner(StandardGameState);

		// Register the new timeline
		StandardGameState->RegisterGameRoundTimeline(Timeline);
	}
	else
	{
		UE_LOG(LogDogFight, Error, TEXT("Failed to spawn Timeline due to no GameState available."));
	}

	// Spawn CardPool
	if (CardPoolClass != nullptr)
	{
		CardPool = NewObject<UGameplayCardPool>(this, CardPoolClass, FName(TEXT("CardPool")));
	}
}

void AStandardGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// Set initial game state to 'EnteringMap'
	SetGamePhase(GamePhase::EnteringMap);
}

void AStandardGameMode::PlayerReadyForGame(const FString& PlayerName)
{
	// Broadcast player joined message
	TArray<FString> Arguments;
	Arguments.Add(FString::Printf(TEXT("<PlayerName>%s</>"),*PlayerName));

	const FGameMessage NewMessage{TEXT("System"), EGameMessageType::GMT_System, TEXT("GameMsg_PlayerJoined"), Arguments};
	BroadcastGameMessageToAllPlayers(NewMessage);

	PlayerJoinedGame++;
	OnJoinedPlayerCountChanged();
}

void AStandardGameMode::RegisterPlayerToTimeline(int32 PlayerId, FString PlayerName)
{
	if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
	{
		if (AGameRoundsTimeline* Timeline = StandardGameState->GetGameRoundsTimeline())
		{
			Timeline->RegisterPlayer(PlayerId, PlayerName);
		}
	}
}

AStandardModePlayerController* AStandardGameMode::GetPlayerControllerById(int32 PlayerId)
{
	for (AStandardModePlayerController* PlayerController : StandardPlayerControllerList)
	{
		if (AStandardPlayerState* PlayerState = PlayerController->GetPlayerState<AStandardPlayerState>())
		{
			if (PlayerState->GetPlayerId() == PlayerId)
			{
				return PlayerController;
			}
		}
	}

	UE_LOG(LogDogFight, Error, TEXT("Failed to get the PlayerController with Id %d"), PlayerId);
	return nullptr;
}

void AStandardGameMode::GivePlayerCards(int32 PlayerId, int32 CardNum)
{
	if (CardPool == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("No CardPool available."));
		return;
	}

	ACardBase* Card = nullptr;
	if (AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(PlayerId))
	{
		if (AStandardPlayerState* PlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
		{
			for (int i = 0; i < CardNum; ++i)
			{
				Card = CardPool->GetRandomCard();
				Card->SetOwnerPlayerController(StandardModePlayerController);
				PlayerState->AddCard(Card);
			}
		}
	}
}

void AStandardGameMode::GivePlayerCardsByCardIndex(int32 PlayerId, int32 CardNum, int32 CardIndex)
{
	if (CardPool == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("No CardPool available."));
		return;
	}

	ACardBase* Card = nullptr;
	if (AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(PlayerId))
	{
		if (AStandardPlayerState* PlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
		{
			for (int i = 0; i < CardNum; ++i)
			{
				Card = CardPool->GetCardByIndex(CardIndex);
				Card->SetOwnerPlayerController(StandardModePlayerController);
				PlayerState->AddCard(Card);
			}
		}
	}
}

void AStandardGameMode::StartGame()
{
	SetGamePhase(GamePhase::SpawnPlayers);

	// Send GameStart message
	const FGameMessage NewMessage{TEXT("Server"), EGameMessageType::GMT_System, TEXT("GameMsg_GameStart")};
	BroadcastGameMessageToAllPlayers(NewMessage);
}

void AStandardGameMode::BroadcastGameMessageToAllPlayers(FGameMessage Message)
{
	for (AStandardModePlayerController* PlayerController : StandardPlayerControllerList)
	{
		PlayerController->RpcReceivedGameMessage(Message);
	}
}

void AStandardGameMode::EndCurrentPlayerRound()
{
	if (CurrentGamePhase == GamePhase::PlayerRound)
	{
		SetGamePhase(GamePhase::PlayerRoundEnd);
	}
}

int32 AStandardGameMode::GetCurrentPlayerId() const
{
	if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
	{
		return StandardGameState->GetGameRoundsTimeline()->GetCurrentPlayerId();
	}

	UE_LOG(LogDogFight, Error, TEXT("Failed to get the StandardGameState."));
	return -1;
}

void AStandardGameMode::BeginPlay()
{
	Super::BeginPlay();

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
			else if (CurrentGamePhase == GamePhase::FreeMoving)
			{
				// Disable click movement
				DisablePlayerClickMovement();

				SetGamePhase(GamePhase::DecideOrder);
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
	else if (CurrentGamePhase == GamePhase::GameSummary)
	{
		HandlePhaseGameSummary();
	}
	else if (CurrentGamePhase == GamePhase::WaitingPostMatch)
	{
		HandlePhaseWaitingPostMatch();
	}
	else if (CurrentGamePhase == GamePhase::SpawnPlayers)
	{
		HandlePhaseSpawnPlayers();
	}
	else if (CurrentGamePhase == GamePhase::FreeMoving)
	{
		HandlePhaseFreeMoving();
	}
	else if (CurrentGamePhase == GamePhase::DecideOrder)
	{
		HandlePhaseDecideOrder();
	}
	else if (CurrentGamePhase == GamePhase::PlayerRoundBegin)
	{
		HandlePhasePlayerRoundBegin();
	}
	else if (CurrentGamePhase == GamePhase::PlayerRound)
	{
		HandlePhasePlayerRound();
	}
	else if (CurrentGamePhase == GamePhase::PlayerRoundEnd)
	{
		HandlePhasePlayerRoundEnd();
	}
	else if (CurrentGamePhase == GamePhase::CheckGameEnd)
	{
		HandlePhaseCheckGameEnd();
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

void AStandardGameMode::HandlePhaseGameSummary()
{
	// Let all player hide the card display widget
	for (AStandardModePlayerController* StandardModePlayerController : StandardPlayerControllerList)
	{
		StandardModePlayerController->RpcHideCardDisplayWidget();
	}

	// Get the winner
	AStandardGameState* StandardGameState = GetGameState<AStandardGameState>();
	AStandardPlayerState* WinnerState = nullptr;
	if (StandardGameState->GetAlivePlayerCount() > 0)
	{
		for (APlayerState* PlayerState: StandardGameState->PlayerArray)
		{
			if (AStandardPlayerState* StandardPlayerState = Cast<AStandardPlayerState>(PlayerState))
			{
				if (StandardPlayerState->IsAlive())
				{
					WinnerState = StandardPlayerState;
					break;
				}
			}
		}
	}
	// Broadcast winner message
	TArray<FString> Arguments;
	if (WinnerState != nullptr)
	{
		Arguments.Add(FString::Printf(TEXT("<PlayerName>%s</>"),*WinnerState->GetPlayerName()));
		const FGameMessage WinnerMessage{TEXT("System"), EGameMessageType::GMT_System, TEXT("GameMsg_Winner"), Arguments};
		BroadcastGameMessageToAllPlayers(WinnerMessage);
	}
	else
	{
		// Consider no survivor as a draw
		const FGameMessage DrawMessage{TEXT("System"), EGameMessageType::GMT_System, TEXT("GameMsg_Draw"), Arguments};
		BroadcastGameMessageToAllPlayers(DrawMessage);
	}
}

void AStandardGameMode::HandlePhaseWaitingPostMatch()
{
}

void AStandardGameMode::HandlePhaseSpawnPlayers()
{
	CurrentSpawnPlayerIndex = 0;
	// Setup timer for spawn all player characters with interval
	GetWorldTimerManager().SetTimer(SecondaryTimerHandle, this, &AStandardGameMode::SpawnPlayerTick, SpawnPlayerInterval, true);
}

void AStandardGameMode::HandlePhaseDecideOrder()
{
	if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
	{
		StandardGameState->GetGameRoundsTimeline()->RandomizeOrder();
		StandardGameState->GetGameRoundsTimeline()->SortTimelineByIndex();

		// Let all clients setup their Timeline widget
		for (AStandardModePlayerController* PlayerController : StandardPlayerControllerList)
		{
			PlayerController->RpcSetupTimelineDisplay();
		}
	}

	SetGamePhase(GamePhase::PlayerRoundBegin);
}

void AStandardGameMode::HandlePhasePlayerRoundBegin()
{
	AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(GetCurrentPlayerId());
	if (StandardModePlayerController == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("Failed to get PlayerController with Id %d"), GetCurrentPlayerId());
		return;
	}

	// Enable card selection for new player
	StandardModePlayerController->RpcSetCardDisplayWidgetSelectable(true);

	if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
	{
		// Reset player state for new round
		StandardPlayerState->InitializePlayerForNewRound();
		// Give current player random cards
		GivePlayerCards(GetCurrentPlayerId(), StandardPlayerState->GetCardGainNumByRound());

		// Register card finished delegate
		StandardPlayerState->OnUsingCardFinished.AddDynamic(this, &AStandardGameMode::OnPlayerUsingCardFinished);
	}

	SetGamePhase(GamePhase::PlayerRound);
}

void AStandardGameMode::HandlePhasePlayerRound()
{
	if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
	{
		AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(StandardGameState->GetGameRoundsTimeline()->GetCurrentPlayerId());
		if (StandardModePlayerController != nullptr)
		{
			StandardModePlayerController->RpcShowCardDisplayWidgetWithSelectMode(ECardSelectionMode::CSM_SingleNoConfirm);
		}
	}
}

void AStandardGameMode::HandlePhasePlayerRoundEnd()
{
	AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(GetCurrentPlayerId());
	if (StandardModePlayerController == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("Failed to get PlayerController with Id %d"), GetCurrentPlayerId());
		return;
	}

	// Disable card selection for ended player
	StandardModePlayerController->RpcSetCardDisplayWidgetSelectable(false);

	// Broadcast event
	OnPlayerRoundEnd.Broadcast(GetCurrentPlayerId());

	if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
	{
		// Remove the card finished delegate
		StandardPlayerState->OnUsingCardFinished.RemoveDynamic(this, &AStandardGameMode::OnPlayerUsingCardFinished);
	}

	// Goto check phase
	SetGamePhase(GamePhase::CheckGameEnd);
}

void AStandardGameMode::HandlePhaseCheckGameEnd()
{
	// Check current alive players
	if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
	{
		if (StandardGameState->GetAlivePlayerCount() <= 1)
		{
			SetGamePhase(GamePhase::GameSummary);
		}
		else
		{
			StandardGameState->GetGameRoundsTimeline()->StepForward();
			SetGamePhase(GamePhase::PlayerRoundBegin);
		}
	}
}

void AStandardGameMode::SpawnPlayerTick()
{
	// Skip and clear the timer if the index is out of range
	if (CurrentSpawnPlayerIndex >= PlayerControllerList.Num())
	{
		GetWorldTimerManager().ClearTimer(SecondaryTimerHandle);

		if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
		{
			StandardGameState->SetAlivePlayerCount(StandardPlayerControllerList.Num());
		}

		// Go to next phase
		SetGamePhase(GamePhase::FreeMoving);
		return;
	}

	// Spawn character for current player
	StandardPlayerControllerList[CurrentSpawnPlayerIndex]->GameStart();
	CurrentSpawnPlayerIndex++;
}

void AStandardGameMode::HandlePhaseFreeMoving()
{
	// Enable click move for all players
	EnablePlayerClickMovement();

	// Set the countdown
	AStandardGameState* StandardGameState = Cast<AStandardGameState>(GameState);
	if (StandardGameState != nullptr && StandardGameState->GetRemainingTime() == 0)
	{
		if (FreeMovingDuration > 0)
		{
			StandardGameState->SetRemainingTime(FreeMovingDuration);
		}
	}
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

void AStandardGameMode::OnPlayerUsingCardFinished(bool bShouldEndRound)
{
	if (bShouldEndRound)
	{
		EndCurrentPlayerRound();
	}
	else
	{
		// Re-enable the card display widget using functionality
		if (AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(GetCurrentPlayerId()))
		{
			StandardModePlayerController->RpcSetCardDisplayWidgetSelectable(true);
		}
	}
}

void AStandardGameMode::OnPlayerDeadCallback(int32 PlayerId)
{
	// Decrease the alive player count
	if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
	{
		StandardGameState->SetAlivePlayerCount(StandardGameState->GetAlivePlayerCount() - 1);

		PushDelayAction(DA_PlayerCountCheck);
	}

	// Broadcast dead event
	OnPlayerDead.Broadcast(PlayerId);

	// Check if is current player dead
	if (GetCurrentPlayerId() == PlayerId)
	{
		EndCurrentPlayerRound();
	}
}

void AStandardGameMode::HandleDelayAction(EGameModeDelayAction DelayAction)
{
	switch (DelayAction)
	{
	case DA_PlayerCountCheck:
		if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
		{
			// If no enough player to continue the game, jump to GameSummary phase.
			if (StandardGameState->GetAlivePlayerCount() <= 1)
			{
				SetGamePhase(GamePhase::GameSummary);
			}
		}
		break;
	default: ;
	}
}

void AStandardGameMode::PushDelayAction(EGameModeDelayAction DelayAction)
{
	if (!DelayActionQueue.Contains(DelayAction))
	{
		DelayActionQueue.Add(DelayAction);
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

void AStandardGameMode::GivePlayerCard(int32 PlayerId, int32 CardNum, int32 CardIndex)
{
	GivePlayerCardsByCardIndex(PlayerId, CardNum, CardIndex);
}


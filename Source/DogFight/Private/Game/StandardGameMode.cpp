// Dog Fight Game Code By CYM.


#include "StandardGameMode.h"


#include "CardBase.h"
#include "DogFight.h"
#include "DogFightGameInstance.h"
#include "StandardGameState.h"
#include "StandardHUD.h"
#include "StandardModePlayerController.h"
#include "StandardModePlayerCharacter.h"
#include "StandardModePlayerPawn.h"
#include "StandardPlayerState.h"
#include "GameplayCardPool.h"
#include "GameRoundsTimeline.h"
#include "StandardModeAIController.h"

namespace GamePhase
{
	const FName EnteringMap = FName(TEXT("EnteringMap"));
	const FName WaitingForStart = FName(TEXT("WaitingForStart"));
	const FName SpawnPlayers = FName(TEXT("SpawnPlayers"));
	const FName SpawnAIs = FName(TEXT("SpawnAIs"));
	const FName FreeMoving = FName(TEXT("FreeMoving"));
	const FName DecideOrder = FName(TEXT("DecideOrder"));
	const FName PlayerRoundBegin = FName(TEXT("PlayerRoundBegin"));
	const FName PlayerRound = FName(TEXT("PlayerRound"));
	const FName DiscardCards = FName(TEXT("DiscardCards"));
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
	GameRoundInterval = 2;

	// Enable the primary tick
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AStandardGameMode::EnablePlayerClickMovement()
{
	for (AStandardModePlayerController* PlayerController : StandardPlayerControllerList)
	{
		PlayerController->ClientSetClickMovementEnabled(true);
	}

	for (AStandardModeAIController* AIController : StandardAIControllerList)
	{
		AIController->EnableFreeMovement();
	}
}

void AStandardGameMode::DisablePlayerClickMovement()
{
	for (AStandardModePlayerController* PlayerController : StandardPlayerControllerList)
	{
		PlayerController->ClientSetClickMovementEnabled(false);
		// Also stop current movement
		PlayerController->StopCharacterMovementImmediately();
	}

	for (AStandardModeAIController* AIController : StandardAIControllerList)
	{
		AIController->DisableFreeMovement();
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
			StandardModePlayerController->OnPlayerHealthChanged.AddDynamic(this, &AStandardGameMode::OnCharacterHealthChangedCallback);
			UE_LOG(LogDogFight, Log, TEXT("Add controller [%s] to list."), *StandardModePlayerController->GetName());
		}
	}
}

void AStandardGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	GetWorldTimerManager().SetTimer(DefaultTimerHandle, this, &AStandardGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);

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

float AStandardGameMode::CalculateDamage(AActor* DamageTaker, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float FinalDamage = Super::CalculateDamage(DamageTaker, Damage, DamageEvent, EventInstigator, DamageCauser); 

	// Record the damage to PlayerState if taker and source are both PlayerCharacter
	AStandardModePlayerCharacter* PlayerCharacter = Cast<AStandardModePlayerCharacter>(DamageTaker);
	AStandardModePlayerController* SourcePlayerController = Cast<AStandardModePlayerController>(EventInstigator);
	if (PlayerCharacter && SourcePlayerController)
	{
		AStandardPlayerState* SourcePlayerState = SourcePlayerController->GetPlayerState<AStandardPlayerState>();
		AStandardPlayerState* StandardPlayerState = Cast<AStandardPlayerState>(PlayerCharacter->GetPlayerState());
		if (StandardPlayerState && SourcePlayerState)
		{
			StandardPlayerState->RecordReceivedDamage(SourcePlayerState->GetPlayerId(), FinalDamage);
		}
	}

	return FinalDamage;
}

void AStandardGameMode::PlayerReadyForGame(const FString& PlayerName)
{
	// Broadcast player joined message
	TArray<FText> Arguments;
	Arguments.Add(FText::FromString(FString::Printf(TEXT("<PlayerName>%s</>"),*PlayerName)));

	const FGameMessage NewMessage{TEXT("System"), EGameMessageType::GMT_System, TEXT("GameMsg_PlayerJoined"), Arguments};
	BroadcastGameMessageToAllPlayers(NewMessage);

	PlayerJoinedGame++;
	OnJoinedPlayerCountChanged();
}

void AStandardGameMode::RegisterPlayerToTimeline(AStandardModePlayerController* PlayerController)
{
	if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
	{
		if (AGameRoundsTimeline* Timeline = StandardGameState->GetGameRoundsTimeline())
		{
			Timeline->RegisterPlayer(PlayerController);
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

	UE_LOG(LogDogFight, Warning, TEXT("Failed to get the PlayerController with Id %d"), PlayerId);
	return nullptr;
}

AStandardModeAIController* AStandardGameMode::GetAIControllerById(int32 PlayerId)
{
	for (AStandardModeAIController* AIController : StandardAIControllerList)
	{
		if (AStandardPlayerState* PlayerState = AIController->GetPlayerState<AStandardPlayerState>())
		{
			if (PlayerState->GetPlayerId() == PlayerId)
			{
				return AIController;
			}
		}
	}

	UE_LOG(LogDogFight, Warning, TEXT("Failed to get the AIController with Id %d"), PlayerId);
	return nullptr;
}

void AStandardGameMode::RegisterAIController(AStandardModeAIController* NewController)
{
	if (StandardAIControllerList.Contains(NewController))
	{
		UE_LOG(LogDogFight, Error, TEXT("Trying to register duplicated AIController to GameMode."));
		return;
	}

	StandardAIControllerList.Add(NewController);

	// Register delegate
	NewController->OnAIPlayerDead.AddDynamic(this, &AStandardGameMode::OnAIPlayerDeadCallback);
	NewController->OnPlayerHealthChanged.AddDynamic(this, &AStandardGameMode::OnCharacterHealthChangedCallback);
}

void AStandardGameMode::RegisterAIToTimeline(AStandardModeAIController* AIController)
{
	if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
	{
		if (AGameRoundsTimeline* Timeline = StandardGameState->GetGameRoundsTimeline())
		{
			Timeline->RegisterAI(AIController);
		}
	}
}

void AStandardGameMode::GivePlayerCards(int32 PlayerId, int32 CardNum)
{
	if (CardPool == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("No CardPool available."));
		return;
	}

	
	if (!bIsCurrentAIPlayer)
	{
		// Handle human player
		if (AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(PlayerId))
		{
			if (AStandardPlayerState* PlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
			{
				GivePlayerCards(StandardModePlayerController, PlayerState, CardNum);
			}
		}
	}
	else
	{
		// Handle AI player
		if (AStandardModeAIController* StandardModeAIController = GetAIControllerById(PlayerId))
		{
			if (AStandardPlayerState* PlayerState = StandardModeAIController->GetPlayerState<AStandardPlayerState>())
			{
				GivePlayerCards(StandardModeAIController, PlayerState, CardNum);
			}
		}
	}
}

void AStandardGameMode::GivePlayerCards(AController* TargetController, AStandardPlayerState* TargetPlayerState, int32 CardNum)
{
	ACardBase* Card = nullptr;
	for (int i = 0; i < CardNum; ++i)
	{
		Card = CardPool->GetRandomCard();
		Card->SetOwnerPlayerController(TargetController);
		TargetPlayerState->AddCard(Card);
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
		PlayerController->ClientReceivedGameMessage(Message);
	}
}

void AStandardGameMode::BroadcastGameTitleMessageToAllPlayers(FGameTitleMessage TitleMessage)
{
	for (AStandardModePlayerController* PlayerController : StandardPlayerControllerList)
	{
		PlayerController->ClientReceivedGameTitleMessage(TitleMessage);
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

AController* AStandardGameMode::GetRandomController()
{
	const int32 TargetIndex = FMath::RandRange(0, StandardPlayerControllerList.Num() + StandardAIControllerList.Num() - 1);
	if (TargetIndex < StandardPlayerControllerList.Num())
	{
		return StandardPlayerControllerList[TargetIndex];
	}
	else
	{
		return StandardAIControllerList[TargetIndex - StandardPlayerControllerList.Num()];
	}
}

void AStandardGameMode::AddPlayerInRagdoll(int32 PlayerId)
{
	if (!CurrentRagdollPlayerId.Contains(PlayerId))
	{
		CurrentRagdollPlayerId.Add(PlayerId);
	}
}

void AStandardGameMode::RemovePlayerInRagdoll(int32 PlayerId)
{
	if (CurrentRagdollPlayerId.Contains(PlayerId))
	{
		CurrentRagdollPlayerId.Remove(PlayerId);
	}
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
			else if (CurrentGamePhase == GamePhase::CheckGameEnd)
			{
				CheckGameEndAction();
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
	else if (CurrentGamePhase == GamePhase::SpawnAIs)
	{
		HandlePhaseSpawnAIs();
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
		StandardModePlayerController->ClientHideCardDisplayWidget();
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
	TArray<FText> Arguments;
	if (WinnerState != nullptr)
	{
		Arguments.Add(FText::FromString(FString::Printf(TEXT("<PlayerName>%s</>"),*WinnerState->GetPlayerName())));
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

void AStandardGameMode::HandlePhaseSpawnAIs()
{
	CurrentSpawnPlayerIndex = 0;
	// Setup timer for spawn all AI with interval
	GetWorldTimerManager().SetTimer(SecondaryTimerHandle, this, &AStandardGameMode::SpawnAITick, SpawnPlayerInterval, true);
}

void AStandardGameMode::HandlePhaseDecideOrder()
{
	if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
	{
		StandardGameState->GetGameRoundsTimeline()->RandomizeOrder();
		StandardGameState->GetGameRoundsTimeline()->SortTimelineByIndex();

		// Debug Timeline
		StandardGameState->GetGameRoundsTimeline()->DebugTimeline();

		// Check if current is AI player
		bIsCurrentAIPlayer = StandardGameState->GetGameRoundsTimeline()->IsCurrentAIPlayer();

		// Let all clients setup their Timeline widget
		for (AStandardModePlayerController* PlayerController : StandardPlayerControllerList)
		{
			PlayerController->ClientSetupTimelineDisplay();

			// Register players statistic
			if (AStandardPlayerState* StandardPlayerState = PlayerController->GetPlayerState<AStandardPlayerState>())
			{
				StandardPlayerState->RegisterPlayersForRelation();
			}
		}

		// Register players statistic for AI
		for (AStandardModeAIController* AIController : StandardAIControllerList)
		{
			if (AStandardPlayerState* StandardPlayerState = AIController->GetPlayerState<AStandardPlayerState>())
			{
				StandardPlayerState->RegisterPlayersForRelation();
			}
		}
	}

	SetGamePhase(GamePhase::PlayerRoundBegin);
}

void AStandardGameMode::HandlePhasePlayerRoundBegin()
{
	TArray<FString> NewRoundMessageArgument;

	if (!bIsCurrentAIPlayer)
	{
		// Handle human player
		AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(GetCurrentPlayerId());
		if (StandardModePlayerController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get PlayerController with Id %d"), GetCurrentPlayerId());
			return;
		}

		// Enable card selection for new player
		StandardModePlayerController->ClientSetCardDisplayWidgetSelectable(true);

		if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
		{
			// Reset player state for new round
			StandardPlayerState->InitializePlayerForNewRound();
			// Give current player random cards
			GivePlayerCards(GetCurrentPlayerId(), StandardPlayerState->GetCardGainNumByRound());

			// Register card finished delegate
			StandardPlayerState->OnUsingCardFinished.AddDynamic(this, &AStandardGameMode::OnPlayerUsingCardFinished);

			NewRoundMessageArgument.Add(StandardPlayerState->GetPlayerName());
		}
	}
	else
	{
		// Handle AI player
		AStandardModeAIController* StandardModeAIController = GetAIControllerById(GetCurrentPlayerId());
		if (StandardModeAIController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get AIController with Id %d"), GetCurrentPlayerId());
			return;
		}

		if (AStandardPlayerState* StandardPlayerState = StandardModeAIController->GetPlayerState<AStandardPlayerState>())
		{
			// Reset player state for new round
			StandardPlayerState->InitializePlayerForNewRound();
			// Give current player random cards
			GivePlayerCards(GetCurrentPlayerId(), StandardPlayerState->GetCardGainNumByRound());

			// Register card finished delegate
			StandardPlayerState->OnUsingCardFinished.AddDynamic(this, &AStandardGameMode::OnPlayerUsingCardFinished);

			NewRoundMessageArgument.Add(StandardPlayerState->GetPlayerName());
		}
	}

	// Broadcast title message
	BroadcastGameTitleMessageToAllPlayers(FGameTitleMessage {FString(TEXT("TitleMsg_PlayerRoundBegin")), NewRoundMessageArgument});

	SetGamePhase(GamePhase::PlayerRound);
}

void AStandardGameMode::HandlePhasePlayerRound()
{
	if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
	{
		if (!bIsCurrentAIPlayer)
		{
			// Show card use Widget
			AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(StandardGameState->GetGameRoundsTimeline()->GetCurrentPlayerId());
			if (StandardModePlayerController != nullptr)
			{
				StandardModePlayerController->ClientShowCardDisplayWidgetWithSelectMode(ECardSelectionMode::CSM_SingleNoConfirm);
			}
		}
		else
		{
			AStandardModeAIController* StandardModeAIController = GetAIControllerById(StandardGameState->GetGameRoundsTimeline()->GetCurrentPlayerId());
			if (StandardModeAIController != nullptr)
			{
				// Notify AIController round started
				StandardModeAIController->StartAIRound();
			}
		}
	}
}

void AStandardGameMode::HandlePhaseDiscardCards()
{
	if (!bIsCurrentAIPlayer)
	{
		// Handle human player
		AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(GetCurrentPlayerId());
		if (StandardModePlayerController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get PlayerController with Id %d"), GetCurrentPlayerId());
			return;
		}

		
	}
	else
	{
		
	}
}

void AStandardGameMode::HandlePhasePlayerRoundEnd()
{
	TArray<FString> NewRoundEndArguments;

	if (!bIsCurrentAIPlayer)
	{
		// Handle human player
		AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(GetCurrentPlayerId());
		if (StandardModePlayerController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get PlayerController with Id %d"), GetCurrentPlayerId());
			return;
		}

		// Disable card selection for ended player
		StandardModePlayerController->ClientSetCardDisplayWidgetSelectable(false);

		// Broadcast event
		OnPlayerRoundEnd.Broadcast(GetCurrentPlayerId());

		if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
		{
			// Remove the card finished delegate
			StandardPlayerState->OnUsingCardFinished.RemoveDynamic(this, &AStandardGameMode::OnPlayerUsingCardFinished);

			NewRoundEndArguments.Add(StandardPlayerState->GetPlayerName());
		}
	}
	else
	{
		// Handle AI player
		AStandardModeAIController* StandardModeAIController = GetAIControllerById(GetCurrentPlayerId());
		if (StandardModeAIController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get AIController with Id %d"), GetCurrentPlayerId());
			return;
		}

		// End AI player round
		StandardModeAIController->StopAIRound();

		// Broadcast event
		OnPlayerRoundEnd.Broadcast(GetCurrentPlayerId());

		if (AStandardPlayerState* StandardPlayerState = StandardModeAIController->GetPlayerState<AStandardPlayerState>())
		{
			// Remove the card finished delegate
			StandardPlayerState->OnUsingCardFinished.RemoveDynamic(this, &AStandardGameMode::OnPlayerUsingCardFinished);

			NewRoundEndArguments.Add(StandardPlayerState->GetPlayerName());
		}
	}

	// Broadcast round end message
	BroadcastGameTitleMessageToAllPlayers(FGameTitleMessage {FString(TEXT("TitleMsg_PlayerRoundEnd")), NewRoundEndArguments});

	// Goto check phase
	SetGamePhase(GamePhase::CheckGameEnd);
}

void AStandardGameMode::HandlePhaseCheckGameEnd()
{
	AStandardGameState* StandardGameState = Cast<AStandardGameState>(GameState);
	if (StandardGameState != nullptr && StandardGameState->GetRemainingTime() == 0)
	{
		if (GameRoundInterval > 0)
		{
			StandardGameState->SetRemainingTime(GameRoundInterval);
		}
		else
		{
			CheckGameEndAction();
		}
	}
}

void AStandardGameMode::CheckGameEndAction()
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
			// Check AI
			bIsCurrentAIPlayer = StandardGameState->GetGameRoundsTimeline()->IsCurrentAIPlayer();

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
		if (UDogFightGameInstance* DogFightGameInstance = Cast<UDogFightGameInstance>(GetGameInstance()))
		{
			if (DogFightGameInstance->GameAICount > 0)
			{
				SetGamePhase(GamePhase::SpawnAIs);
			}
			else
			{
				SetGamePhase(GamePhase::FreeMoving);
			}
		}
		else
		{
			UE_LOG(LogDogFight, Error, TEXT("Invalid GameInstance class. (Expects DogFightGameInstance or its subclass.)"));
		}
		return;
	}

	// Spawn character for current player
	StandardPlayerControllerList[CurrentSpawnPlayerIndex]->GameStart();
	CurrentSpawnPlayerIndex++;
}

void AStandardGameMode::SpawnAITick()
{
	int32 AICount = 0;
	if (UDogFightGameInstance* DogFightGameInstance = Cast<UDogFightGameInstance>(GetGameInstance()))
	{
		AICount = DogFightGameInstance->GameAICount;
	}
	
	if (CurrentSpawnPlayerIndex >= AICount)
	{
		GetWorldTimerManager().ClearTimer(SecondaryTimerHandle);
		
		if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
		{
			StandardGameState->SetAlivePlayerCount(StandardGameState->GetAlivePlayerCount() + AICount);
		}

		SetGamePhase(GamePhase::FreeMoving);
		return;
	}

	// Spawn AI Controller
	if (IsValid(AIControllerClass))
	{
		GetWorld()->SpawnActor(AIControllerClass);
	}
	else
	{
		UE_LOG(LogDogFight, Error, TEXT("No valid AIController class specified."));
	}
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
		if (!bIsCurrentAIPlayer)
		{
			// Re-enable the card display widget using functionality
			if (AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(GetCurrentPlayerId()))
			{
				StandardModePlayerController->ClientSetCardDisplayWidgetSelectable(true);
			}
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

void AStandardGameMode::OnAIPlayerDeadCallback(int32 PlayerId)
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

void AStandardGameMode::OnCharacterHealthChangedCallback(int32 PlayerId, int32 NewHealth)
{
	// Notify all players the health changes
	if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
	{
		for (APlayerState* PlayerState : StandardGameState->PlayerArray)
		{
			if (AStandardPlayerState* StandardPlayerState = Cast<AStandardPlayerState>(PlayerState))
			{
				StandardPlayerState->ModifyPlayerHealth(PlayerId, NewHealth);
			}
		}
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


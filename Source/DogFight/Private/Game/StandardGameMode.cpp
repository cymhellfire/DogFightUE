// Dog Fight Game Code By CYM.


#include "Game/StandardGameMode.h"

#include "Card/CardBase.h"
#include "DogFight.h"
#include "Actors/Managers/BuffQueue.h"
#include "Game/DogFightGameInstance.h"
#include "Game/StandardGameState.h"
#include "UI/StandardHUD.h"
#include "Player/StandardModePlayerController.h"
#include "Pawns/StandardModePlayerCharacter.h"
#include "Pawns/StandardModePlayerPawn.h"
#include "Game/StandardPlayerState.h"
#include "Game/GameplayCardPool.h"
#include "Game/GameRoundsTimeline.h"
#include "AI/StandardModeAIController.h"
#include "Common/BitmaskOperation.h"
#include "Actors/Weapons/WeaponBase.h"

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
	const FName CharacterReturn = FName(TEXT("CharacterReturn"));
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

	// Player settings
	DefaultCardCapacity = 2;
	CardCapacityRange = FInt32Range(1, 8);
	DefaultCardUsingCount = 2;
	CardUsingCountRange = FInt32Range(1, 8);
	DefaultCardGainPerRound = 2;
	CardGainPerRoundRange = FInt32Range(1, 6);

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
	AStandardModeAIController* SourceAIController = Cast<AStandardModeAIController>(EventInstigator);
	AStandardPlayerState* SourcePlayerState =
		SourcePlayerController ? SourcePlayerController->GetPlayerState<AStandardPlayerState>()
		: SourceAIController ? SourceAIController->GetPlayerState<AStandardPlayerState>()
		: nullptr;
	if (PlayerCharacter && SourcePlayerState)
	{
		AStandardPlayerState* StandardPlayerState = Cast<AStandardPlayerState>(PlayerCharacter->GetPlayerState());
		if (StandardPlayerState)
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
		//SetGamePhase(GamePhase::PlayerRoundEnd);
		//SetGamePhase(GamePhase::DiscardCards);
		SetGamePhase(GamePhase::CharacterReturn);
	}
	else if (CurrentGamePhase == GamePhase::PlayerRoundBegin)
	{
		SetGamePhase(GamePhase::PlayerRoundEnd);
	}
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

AStandardModePlayerCharacter* AStandardGameMode::GetPlayerCharacterById(int32 PlayerId)
{
	if (AStandardModePlayerController* PlayerController = GetPlayerControllerById(PlayerId))
	{
		return PlayerController->GetCharacterPawn();
	}
	else if (AStandardModeAIController* AIController = GetAIControllerById(PlayerId))
	{
		return AIController->GetCharacterPawn();
	}

	return nullptr;
}

int32 AStandardGameMode::ClampCardUsingCount(int32 InValue)
{
	return FMath::Clamp(InValue, CardUsingCountRange.GetLowerBoundValue(), CardUsingCountRange.GetUpperBoundValue());
}

int32 AStandardGameMode::ClampCardCapacity(int32 InValue)
{
	return FMath::Clamp(InValue, CardCapacityRange.GetLowerBoundValue(), CardCapacityRange.GetUpperBoundValue());
}

int32 AStandardGameMode::ClampCardGainPerRound(int32 InValue)
{
	return FMath::Clamp(InValue, CardGainPerRoundRange.GetLowerBoundValue(), CardGainPerRoundRange.GetUpperBoundValue());
}

void AStandardGameMode::RequestResponseCardFromPlayer(int32 PlayerId, TArray<TSubclassOf<ACardBase>> ResponseCardClasses, AActor* SourceActor, const FText& CardToResponse)
{
	const bool bTargetHumanPlayer = IsValid(GetPlayerControllerById(PlayerId));

	if (bTargetHumanPlayer)
	{
		// Handle human player
		AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(PlayerId);
		if (StandardModePlayerController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get PlayerController with Id %d"), GetCurrentPlayerId());
			return;
		}

		if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
		{
			// Apply card using filter
			StandardPlayerState->ClearCardUsableFilter();
			StandardPlayerState->ApplyCardUsableFilterByClass(ResponseCardClasses);
			StandardPlayerState->ApplyCardUsableFilterByUseMethod(ECardUseMethod::CUM_Passive);

			// Check if there is any card can response
			if (StandardPlayerState->GetUsableCardCount() > 0)
			{
				// Face to incoming enemy
				if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(StandardModePlayerController->GetActualPawn()))
				{
					const FVector FacingDirection = SourceActor->GetActorLocation() - StandardModePlayerCharacter->GetActorLocation();
					StandardModePlayerCharacter->SetAimingDirection(FacingDirection, [this, StandardPlayerState, StandardModePlayerController, CardToResponse]()
					{
						StandardPlayerState->SetCardSelectionPurpose(ECardSelectionPurpose::CSP_Response);
						StandardPlayerState->OnResponseCardSelected.AddDynamic(this, &AStandardGameMode::OnResponseCardSelected);
						StandardModePlayerController->ClientStartRequestResponseCard(1, CardToResponse);
						StandardModePlayerController->ClientSetCardDisplayWidgetSelectable(true);
					});
				}
			}
			else
			{
				OnResponseCardSelected(nullptr, StandardPlayerState);
			}
		}
	}
	else
	{
		// Handle AI player
		AStandardModeAIController* StandardModeAIController = GetAIControllerById(PlayerId);
		if (StandardModeAIController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get AIController with Id %d"), GetCurrentPlayerId());
			return;
		}

		if (AStandardPlayerState* StandardPlayerState = StandardModeAIController->GetPlayerState<AStandardPlayerState>())
		{
			// Apply card using filter
			StandardPlayerState->ClearCardUsableFilter();
			StandardPlayerState->ApplyCardUsableFilterByClass(ResponseCardClasses);
			StandardPlayerState->ApplyCardUsableFilterByUseMethod(ECardUseMethod::CUM_Passive);

			// Check if there is any card can response
			if (StandardPlayerState->GetUsableCardCount() > 0)
			{
				// Face to incoming enemy
				if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(StandardModeAIController->GetActualPawn()))
				{
					const FVector FacingDirection = SourceActor->GetActorLocation() - StandardModePlayerCharacter->GetActorLocation();
					StandardModePlayerCharacter->SetAimingDirection(FacingDirection, [this, StandardPlayerState, StandardModeAIController]()
					{
						StandardPlayerState->SetCardSelectionPurpose(ECardSelectionPurpose::CSP_Response);
						StandardPlayerState->OnResponseCardSelected.AddDynamic(this, &AStandardGameMode::OnResponseCardSelected);

						StandardModeAIController->UseResponseCard();
					});
				}
			}
			else
			{
				OnResponseCardSelected(nullptr, StandardPlayerState);
			}
		}
	}
}

void AStandardGameMode::OnResponseCardSelected(ACardBase* SelectedCard, AStandardPlayerState* ResponsePlayerState)
{
	// Unregister callback
	if (ResponsePlayerState->OnResponseCardSelected.IsAlreadyBound(this, &AStandardGameMode::OnResponseCardSelected))
	{
		ResponsePlayerState->OnResponseCardSelected.RemoveDynamic(this, &AStandardGameMode::OnResponseCardSelected);
	}

	if (AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(ResponsePlayerState->GetPlayerId()))
	{
		ResponsePlayerState->MarkAllCardUnUsable();
		StandardModePlayerController->ClientSetCardDisplayWidgetSelectable(false);
		StandardModePlayerController->ClientStopRequestResponseCard();
	}

	if (IsValid(SelectedCard))
	{
		// Use card here
		SelectedCard->Use();
	}

	// Broadcast callback
	OnPlayerResponseCardSelected.Broadcast();
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

	UE_LOG(LogDogFight, Log, TEXT("GameMode [Player %d]: Changed to phase [%s]"), GetCurrentPlayerId(), *NewPhase.ToString());

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
	else if (CurrentGamePhase == GamePhase::CharacterReturn)
	{
		HandlePhaseCharacterReturn();
	}
	else if (CurrentGamePhase == GamePhase::DiscardCards)
	{
		HandlePhaseDiscardCards();
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
		CachedCurrentPlayerId = StandardGameState->GetGameRoundsTimeline()->GetCurrentPlayerId();

		// Let all clients setup their Timeline widget
		for (AStandardModePlayerController* PlayerController : StandardPlayerControllerList)
		{
			PlayerController->ClientSetupTimelineDisplay();

			// Register players statistic
			if (AStandardPlayerState* StandardPlayerState = PlayerController->GetPlayerState<AStandardPlayerState>())
			{
				StandardPlayerState->RegisterPlayersForRelation();
			}

			// Equip default weapon
			if (IsValid(CharacterDefaultWeapon))
			{
				if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(PlayerController->GetActualPawn()))
				{
					UWeaponBase* NewWeapon = NewObject<UWeaponBase>(StandardModePlayerCharacter, CharacterDefaultWeapon);
					StandardModePlayerCharacter->OnWeaponEquippedEvent.AddDynamic(this, &AStandardGameMode::AStandardGameMode::OnWeaponEquipped);
					StandardModePlayerCharacter->EquipWeapon(NewWeapon);

					// Also update cache location
					StandardModePlayerCharacter->CacheCurrentLocation();

					WeaponEquipWaitingCharacterCount++;
				}
			}
		}

		// Register players statistic for AI
		for (AStandardModeAIController* AIController : StandardAIControllerList)
		{
			if (AStandardPlayerState* StandardPlayerState = AIController->GetPlayerState<AStandardPlayerState>())
			{
				StandardPlayerState->RegisterPlayersForRelation();
			}

			// Equip default weapon
			if (IsValid(CharacterDefaultWeapon))
			{
				if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(AIController->GetActualPawn()))
				{
					UWeaponBase* NewWeapon = NewObject<UWeaponBase>(StandardModePlayerCharacter, CharacterDefaultWeapon);
					StandardModePlayerCharacter->OnWeaponEquippedEvent.AddDynamic(this, &AStandardGameMode::AStandardGameMode::OnWeaponEquipped);
					StandardModePlayerCharacter->EquipWeapon(NewWeapon);

					// Also update cache location
					StandardModePlayerCharacter->CacheCurrentLocation();

					WeaponEquipWaitingCharacterCount++;
				}
			}
		}
	}

	// If no weapon to equip, just move to next phase
	if (WeaponEquipWaitingCharacterCount == 0)
	{
		SetGamePhase(GamePhase::PlayerRoundBegin);
	}
}

void AStandardGameMode::OnWeaponEquipped(AActor* CarrierActor)
{
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(CarrierActor))
	{
		StandardModePlayerCharacter->OnWeaponEquippedEvent.RemoveDynamic(this, &AStandardGameMode::OnWeaponEquipped);
	}

	WeaponEquipWaitingCharacterCount--;
	if (WeaponEquipWaitingCharacterCount == 0)
	{
		SetGamePhase(GamePhase::PlayerRoundBegin);
	}
}

void AStandardGameMode::HandlePhasePlayerRoundBegin()
{
	// Start Buff Queue process
	if (!bIsCurrentAIPlayer)
	{
		// Handle human player
		AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(GetCurrentPlayerId());
		if (StandardModePlayerController == nullptr)
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to get PlayerController with Id %d"), GetCurrentPlayerId());
			return;
		}

		if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
		{
			if (UBuffQueue* BuffQueue = StandardPlayerState->GetBuffQueue())
			{
				if (BuffQueue->GetBuffCount() > 0)
				{
					BuffQueue->OnBuffQueueProcessFinished.AddDynamic(this, &AStandardGameMode::OnPlayerBuffQueueBeginRoundFinished);
					BuffQueue->StartRoundBeginBuffCheckProcess();
				}
				else
				{
					OnPlayerBuffQueueBeginRoundFinished();
				}
			}
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
			if (UBuffQueue* BuffQueue = StandardPlayerState->GetBuffQueue())
			{
				if (BuffQueue->GetBuffCount() > 0)
				{
					BuffQueue->OnBuffQueueProcessFinished.AddDynamic(this, &AStandardGameMode::OnPlayerBuffQueueBeginRoundFinished);
					BuffQueue->StartRoundBeginBuffCheckProcess();
				}
				else
				{
					OnPlayerBuffQueueBeginRoundFinished();
				}
			}
		}
	}
}

void AStandardGameMode::OnPlayerBuffQueueBeginRoundFinished()
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

		if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
		{
			if (UBuffQueue* BuffQueue = StandardPlayerState->GetBuffQueue())
			{
				if (BuffQueue->OnBuffQueueProcessFinished.IsBound())
				{
					BuffQueue->OnBuffQueueProcessFinished.RemoveDynamic(this, &AStandardGameMode::OnPlayerBuffQueueBeginRoundFinished);
				}
			}

			// Reset player state for new round
			StandardPlayerState->InitializePlayerForNewRound();

			// Skip give card if player is marked with SkipGiveCard
			if (!TEST_SINGLE_FLAG(StandardPlayerState->GetSkipGamePhaseFlags(), ESGP_GiveCards))
			{
				// Give current player random cards
				GivePlayerCards(GetCurrentPlayerId(), StandardPlayerState->GetCardGainNumByRound());
			}

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
			if (UBuffQueue* BuffQueue = StandardPlayerState->GetBuffQueue())
			{
				if (BuffQueue->OnBuffQueueProcessFinished.IsBound())
				{
					BuffQueue->OnBuffQueueProcessFinished.RemoveDynamic(this, &AStandardGameMode::OnPlayerBuffQueueBeginRoundFinished);
				}
			}

			// Reset player state for new round
			StandardPlayerState->InitializePlayerForNewRound();

			// Skip give card if player is marked with SkipGiveCard
			if (!TEST_SINGLE_FLAG(StandardPlayerState->GetSkipGamePhaseFlags(), ESGP_GiveCards))
			{
				// Give current player random cards
				GivePlayerCards(GetCurrentPlayerId(), StandardPlayerState->GetCardGainNumByRound());
			}

			// Register card finished delegate
			StandardPlayerState->OnUsingCardFinished.AddDynamic(this, &AStandardGameMode::OnPlayerUsingCardFinished);

			NewRoundMessageArgument.Add(StandardPlayerState->GetPlayerName());
		}
	}

	// Broadcast title message
	BroadcastGameTitleMessageToAllPlayers(FGameTitleMessage {FString(TEXT("TitleMsg_PlayerRoundBegin")), NewRoundMessageArgument});

	SetGamePhase(GamePhase::PlayerRound);

	OnPlayerRoundBegin.Broadcast(GetCurrentPlayerId());
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
				bool bSkipUsingCardPhase = false;
				if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
				{
					bSkipUsingCardPhase = TEST_SINGLE_FLAG(StandardPlayerState->GetSkipGamePhaseFlags(), ESGP_UseCards);

					if (!bSkipUsingCardPhase)
					{
						StandardPlayerState->SetCardSelectionPurpose(ECardSelectionPurpose::CSP_Use);
						StandardPlayerState->ClearCardUsableFilter();
						StandardPlayerState->ApplyCardUsableFilterByUseMethod(ECardUseMethod::CUM_Aggressive);
					}
				}

				// Directly end current round if player is marked as SkipUsingCard
				if (!bSkipUsingCardPhase)
				{
					// Enable card selection for new player
					StandardModePlayerController->ClientSetCardDisplayWidgetSelectable(true);
					StandardModePlayerController->ClientShowCardDisplayWidgetWithSelectMode(ECardSelectionMode::CSM_SingleNoConfirm);
				}
				else
				{
					EndCurrentPlayerRound();
				}
			}
		}
		else
		{
			AStandardModeAIController* StandardModeAIController = GetAIControllerById(StandardGameState->GetGameRoundsTimeline()->GetCurrentPlayerId());
			if (StandardModeAIController != nullptr)
			{
				bool bSkipUsingCard = false;
				if (AStandardPlayerState* StandardPlayerState = StandardModeAIController->GetPlayerState<AStandardPlayerState>())
				{
					bSkipUsingCard = TEST_SINGLE_FLAG(StandardPlayerState->GetSkipGamePhaseFlags(), ESGP_UseCards);

					if (!bSkipUsingCard)
					{
						StandardPlayerState->SetCardSelectionPurpose(ECardSelectionPurpose::CSP_Use);
						StandardPlayerState->ClearCardUsableFilter();
						StandardPlayerState->ApplyCardUsableFilterByUseMethod(ECardUseMethod::CUM_Aggressive);
					}
				}

				// Directly end current round if player is marked as SkipUsingCard
				if (!bSkipUsingCard)
				{
					// Notify AIController round started
					StandardModeAIController->StartAIRound();
				}
				else
				{
					EndCurrentPlayerRound();
				}
			}
		}
	}
}

void AStandardGameMode::HandlePhaseCharacterReturn()
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

		// Disable all cards for selecting
		if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
		{
			StandardPlayerState->MarkAllCardUnUsable();
		}
	}

	ReturnedCharacterCount = 0;

	// Let all players return
	for (AStandardModePlayerController* PlayerController : StandardPlayerControllerList)
	{
		if (AStandardPlayerState* StandardPlayerState = PlayerController->GetPlayerState<AStandardPlayerState>())
		{
			if (StandardPlayerState->IsAlive())
			{
				if (AStandardModePlayerCharacter* PlayerCharacter = Cast<AStandardModePlayerCharacter>(PlayerController->GetActualPawn()))
				{
					PlayerCharacter->GetCarrierReachActionDistanceEvent().AddDynamic(this, &AStandardGameMode::OnCharacterReturnFinished);
					PlayerCharacter->ReturnToCachedLocation();
				}
			}
		}
	}

	// Let all AI return
	for (AStandardModeAIController* AIController : StandardAIControllerList)
	{
		if (AStandardPlayerState* StandardPlayerState = AIController->GetPlayerState<AStandardPlayerState>())
		{
			if (StandardPlayerState->IsAlive())
			{
				if (AStandardModePlayerCharacter* PlayerCharacter = Cast<AStandardModePlayerCharacter>(AIController->GetActualPawn()))
				{
					PlayerCharacter->GetCarrierReachActionDistanceEvent().AddDynamic(this, &AStandardGameMode::OnCharacterReturnFinished);
					PlayerCharacter->ReturnToCachedLocation();
				}
			}
		}
	}
}

void AStandardGameMode::OnCharacterReturnFinished(AActor* Actor)
{
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(Actor))
	{
		StandardModePlayerCharacter->GetCarrierReachActionDistanceEvent().RemoveDynamic(this, &AStandardGameMode::OnCharacterReturnFinished);
	}

	ReturnedCharacterCount++;

	if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
	{
		if (StandardGameState->GetAlivePlayerCount() == ReturnedCharacterCount)
		{
			SetGamePhase(GamePhase::DiscardCards);
		}
	}
	else
	{
		UE_LOG(LogDogFight, Error, TEXT("Failed to get StandardGameState."));
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

		if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
		{
			// Check whether to discard cards
			const int32 DiscardCount = TEST_SINGLE_FLAG(StandardPlayerState->GetSkipGamePhaseFlags(), ESGP_DropCards) ? 0 : StandardPlayerState->CardCountToDiscard();
			if (DiscardCount > 0 && StandardPlayerState->IsAlive())
			{
				StandardPlayerState->SetCardSelectionPurpose(ECardSelectionPurpose::CSP_Discard);
				// Enable card selection for discarding
				StandardModePlayerController->ClientSetCardDisplayWidgetSelectable(true);
				StandardPlayerState->ClearCardUsableFilter();
				StandardModePlayerController->ClientStartDiscardCards(DiscardCount);

				StandardPlayerState->OnDiscardCardFinished.AddDynamic(this, &AStandardGameMode::AStandardGameMode::OnPlayerDiscardCardFinished);
			}
			else
			{
				SetGamePhase(GamePhase::PlayerRoundEnd);
			}
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
			// Check whether to discard cards
			const int32 DiscardCount = TEST_SINGLE_FLAG(StandardPlayerState->GetSkipGamePhaseFlags(), ESGP_DropCards) ? 0 : StandardPlayerState->CardCountToDiscard();
			if (DiscardCount > 0 && StandardPlayerState->IsAlive())
			{
				StandardModeAIController->DiscardRandomCards(DiscardCount);
			}

			SetGamePhase(GamePhase::PlayerRoundEnd);
		}
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
			// Disable all cards using
			StandardPlayerState->MarkAllCardUnUsable();

			// Remove the card finished delegate
			StandardPlayerState->OnUsingCardFinished.RemoveDynamic(this, &AStandardGameMode::OnPlayerUsingCardFinished);

			NewRoundEndArguments.Add(StandardPlayerState->GetPlayerName());

			// Process buff queue
			if (UBuffQueue* BuffQueue = StandardPlayerState->GetBuffQueue())
			{
				if (BuffQueue->GetBuffCount() > 0)
				{
					BuffQueue->OnBuffQueueProcessFinished.AddDynamic(this, &AStandardGameMode::AStandardGameMode::OnPlayerBuffQueueEndRoundFinished);
					BuffQueue->StartRoundEndBuffCheckProcess();
				}
				else
				{
					OnPlayerBuffQueueEndRoundFinished();
				}
			}
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

			// Process buff queue
			if (UBuffQueue* BuffQueue = StandardPlayerState->GetBuffQueue())
			{
				if (BuffQueue->GetBuffCount() > 0)
				{
					BuffQueue->OnBuffQueueProcessFinished.AddDynamic(this, &AStandardGameMode::AStandardGameMode::OnPlayerBuffQueueEndRoundFinished);
					BuffQueue->StartRoundEndBuffCheckProcess();
				}
				else
				{
					OnPlayerBuffQueueEndRoundFinished();
				}
			}
		}
	}

	// Broadcast round end message
	BroadcastGameTitleMessageToAllPlayers(FGameTitleMessage {FString(TEXT("TitleMsg_PlayerRoundEnd")), NewRoundEndArguments});
}

void AStandardGameMode::OnPlayerBuffQueueEndRoundFinished()
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

		if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
		{
			if (UBuffQueue* BuffQueue = StandardPlayerState->GetBuffQueue())
			{
				if (BuffQueue->OnBuffQueueProcessFinished.IsBound())
				{
					BuffQueue->OnBuffQueueProcessFinished.RemoveDynamic(this, &AStandardGameMode::OnPlayerBuffQueueEndRoundFinished);
				}
			}
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
			if (UBuffQueue* BuffQueue = StandardPlayerState->GetBuffQueue())
			{
				if (BuffQueue->OnBuffQueueProcessFinished.IsBound())
				{
					BuffQueue->OnBuffQueueProcessFinished.RemoveDynamic(this, &AStandardGameMode::OnPlayerBuffQueueEndRoundFinished);
				}
			}
		}
	}

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
			CachedCurrentPlayerId = StandardGameState->GetGameRoundsTimeline()->GetCurrentPlayerId();

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
				if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
				{
					StandardPlayerState->ClearCardUsableFilter();
					StandardPlayerState->ApplyCardUsableFilterByUseMethod(ECardUseMethod::CUM_Aggressive);
				}
			}
		}
		else
		{
			if (AStandardModeAIController* StandardModeAIController = GetAIControllerById(GetCurrentPlayerId()))
			{
				if (AStandardPlayerState* StandardPlayerState = StandardModeAIController->GetPlayerState<AStandardPlayerState>())
				{
					StandardPlayerState->ClearCardUsableFilter();
					StandardPlayerState->ApplyCardUsableFilterByUseMethod(ECardUseMethod::CUM_Aggressive);
				}
				StandardModeAIController->PrepareForUsingCard();
			}
		}
	}
}

void AStandardGameMode::OnPlayerDiscardCardFinished()
{
	if (!bIsCurrentAIPlayer)
	{
		if (AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(GetCurrentPlayerId()))
		{
			StandardModePlayerController->ClientStopDiscardCards();
			if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
			{
				StandardPlayerState->OnDiscardCardFinished.RemoveDynamic(this, &AStandardGameMode::AStandardGameMode::OnPlayerDiscardCardFinished);
			}
		}
	}

	SetGamePhase(GamePhase::PlayerRoundEnd);
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


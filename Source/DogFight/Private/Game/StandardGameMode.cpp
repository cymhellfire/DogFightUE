// Dog Fight Game Code By CYM.


#include "Game/StandardGameMode.h"

#include "Card/CardBase.h"
#include "DogFight.h"
#include "Ability/AbilityBase.h"
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
#include "Game/GameplayAbilityPool.h"
#include "Game/GameWorkflow/GameModeStateMachine.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModeCharacterReturnPhase.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModeCheckGameEndPhase.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModeDecideOrderPhase.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModeDiscardCardsPhase.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModeEnteringMapPhase.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModeFreeMovingPhase.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModeGameSummaryPhase.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModePhaseBase.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModePhaseDefine.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModePlayerRoundBeginPhase.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModePlayerRoundEndPhase.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModePlayerRoundPhase.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModeSelectAbilityPhase.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModeSpawnPlayersPhase.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModeTimedPhase.h"

AStandardGameMode::AStandardGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = AStandardModePlayerPawn::StaticClass();
	HUDClass = AStandardHUD::StaticClass();
	PlayerControllerClass = AStandardModePlayerController::StaticClass();
	GameStateClass = AStandardGameState::StaticClass();
	PlayerStateClass = AStandardPlayerState::StaticClass();

	//CurrentGamePhase = GamePhase::EnteringMap;
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
	InitialAbilityCandidateCount = 3;

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

	// Update state machine
	if (IsValid(GameModeStateMachine))
	{
		GameModeStateMachine->StateMachineTick();
	}

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

	// Remove player from list
	if (APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>())
	{
		if (HumanPlayerIdList.Contains(PlayerState->GetPlayerId()))
		{
			HumanPlayerIdList.Remove(PlayerState->GetPlayerId());
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
			StandardModePlayerController->OnPlayerStrengthChanged.AddDynamic(this, &AStandardGameMode::OnCharacterStrengthChangedCallback);
			StandardModePlayerController->OnPlayerCardTargetAcquired.AddDynamic(this, &AStandardGameMode::OnPlayerCardTargetAcquired);
			UE_LOG(LogDogFight, Log, TEXT("Add controller [%s] to list."), *StandardModePlayerController->GetName());
		}
	}
}

void AStandardGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

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

	// Spawn AbilityPool
	if (AbilityPoolClass != nullptr)
	{
		AbilityPool = NewObject<UGameplayAbilityPool>(this, AbilityPoolClass, FName(TEXT("AbilityPool")));
	}

	// Local player also trigger player count changed
	OnJoinedPlayerCountChanged();
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


FName AStandardGameMode::GetGamePhase() const
{
	if (!IsValid(GameModeStateMachine))
	{
		return NAME_None; 
	}

	return GameModeStateMachine->GetCurrentGamePhaseName();
}

void AStandardGameMode::PlayerReadyForGame(const FString& PlayerName)
{
	// Broadcast player joined message
	TArray<FText> Arguments;
	Arguments.Add(FText::FromString(FString::Printf(TEXT("<PlayerName>%s</>"),*PlayerName)));

	const FGameMessage NewMessage{TEXT("System"), EGameMessageType::GMT_System, TEXT("GameMsg_PlayerJoined"), Arguments};
	BroadcastGameMessageToAllPlayers(NewMessage);

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

	// Record human player id
	if (AStandardPlayerState* PlayerState = PlayerController->GetPlayerState<AStandardPlayerState>())
	{
		const int32 PlayerId = PlayerState->GetPlayerId();
		HumanPlayerIdList.AddUnique(PlayerId);

#if WITH_IMGUI
		// Create player base info struct
		if (!PlayerBaseInfoMap.Contains(PlayerId))
		{
			FDebugPlayerBaseInfo NewPlayerInfo;
			NewPlayerInfo.PlayerId = PlayerId;
			NewPlayerInfo.PlayerName = PlayerState->GetPlayerName();
			PlayerBaseInfoMap.Add(PlayerId, NewPlayerInfo);
		}
		else
		{
			FDebugPlayerBaseInfo* PlayerBaseInfo = PlayerBaseInfoMap.Find(PlayerId);
			PlayerBaseInfo->PlayerName = PlayerState->GetPlayerName();
		}

		// Register listener
		if (!PlayerState->OnPlayerCardCountChanged.IsAlreadyBound(this, &AStandardGameMode::OnPlayerCardCountChanged))
		{
			PlayerState->OnPlayerCardCountChanged.AddDynamic(this, &AStandardGameMode::OnPlayerCardCountChanged);
			PlayerState->OnPlayerRelationInfoChanged.AddDynamic(this, &AStandardGameMode::OnPlayerRelationInfoChanged);
		}
#endif
	}
}

void AStandardGameMode::RevivePlayer(int32 PlayerId)
{
	if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
	{
		StandardGameState->SetAlivePlayerCount(StandardGameState->GetAlivePlayerCount() + 1);
		if (AGameRoundsTimeline* Timeline = StandardGameState->GetGameRoundsTimeline())
		{
			Timeline->RevivePlayer(PlayerId);
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
	NewController->OnPlayerStrengthChanged.AddDynamic(this, &AStandardGameMode::OnCharacterStrengthChangedCallback);
	NewController->OnPlayerCardTargetAcquired.AddDynamic(this, &AStandardGameMode::OnPlayerCardTargetAcquired);

#if WITH_IMGUI
	if (AStandardPlayerState* PlayerState = NewController->GetPlayerState<AStandardPlayerState>())
	{
		const int32 PlayerId = PlayerState->GetPlayerId();
		// Create player base info struct
		FDebugPlayerBaseInfo NewPlayerInfo;
		NewPlayerInfo.PlayerId = PlayerId;
		NewPlayerInfo.PlayerName = PlayerState->GetPlayerName();
		PlayerBaseInfoMap.Add(PlayerId, NewPlayerInfo);

		// Register listener
		if (!PlayerState->OnPlayerCardCountChanged.IsAlreadyBound(this, &AStandardGameMode::OnPlayerCardCountChanged))
		{
			PlayerState->OnPlayerCardCountChanged.AddDynamic(this, &AStandardGameMode::OnPlayerCardCountChanged);
			PlayerState->OnPlayerRelationInfoChanged.AddDynamic(this, &AStandardGameMode::OnPlayerRelationInfoChanged);
		}
	}
#endif
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

	if (IsHumanPlayer(PlayerId))
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

#if WITH_IMGUI
		GetCurrentGamePhaseRecord().AddGiveCardEvent(Card->GetClass()->GetName(), TargetPlayerState->GetPlayerId());
#endif
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

void AStandardGameMode::SendGameMessageToPlayer(FGameMessage Message, int32 PlayerId)
{
	if (AStandardModePlayerController* PlayerController = GetPlayerControllerById(PlayerId))
	{
		PlayerController->ClientReceivedGameMessage(Message);
	}
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
	OnRequestEndCurrentPlayerRound.Broadcast();
}

void AStandardGameMode::SetCurrentPlayerId(int32 NewId)
{
#if WITH_IMGUI
	if (PlayerBaseInfoMap.Contains(CachedCurrentPlayerId))
	{
		FDebugPlayerBaseInfo* PlayerBaseInfo = PlayerBaseInfoMap.Find(CachedCurrentPlayerId);
		PlayerBaseInfo->bActive = false;
	}
#endif

	CachedCurrentPlayerId = NewId;

#if WITH_IMGUI
	if (PlayerBaseInfoMap.Contains(NewId))
	{
		FDebugPlayerBaseInfo* PlayerBaseInfo = PlayerBaseInfoMap.Find(NewId);
		PlayerBaseInfo->bActive = true;
	}
#endif
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

AController* AStandardGameMode::GetControllerById(int32 PlayerId)
{
	if (AStandardModePlayerController* PlayerController = GetPlayerControllerById(PlayerId))
	{
		return PlayerController;
	}
	else if (AStandardModeAIController* AIController = GetAIControllerById(PlayerId))
	{
		return AIController;
	}

	return nullptr;
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

int32 AStandardGameMode::TransferCardsBetweenPlayer(AStandardPlayerState* SrcPlayerState, AStandardPlayerState* DestPlayerState, FTransferCardInfo CardInfo)
{
	TArray<int32> TransferCardList;
	switch (CardInfo.TransferType)
	{
	case TCT_Random:
		SrcPlayerState->GetRandomTransferCards(CardInfo.TransferCardData[0], TransferCardList);
		break;
	case TCT_Specified:
		TransferCardList = CardInfo.TransferCardData;
		break;
	default:
		break;
	}

	for (int32 CardIndex : TransferCardList)
	{
		TransferCardBetweenPlayers_Internal(SrcPlayerState, DestPlayerState, CardIndex);
	}

	return TransferCardList.Num();
}

void AStandardGameMode::BroadcastCameraFocusEvent(FCameraFocusEvent CameraEvent)
{
	OnCameraEventHappened.Broadcast(CameraEvent);
}

void AStandardGameMode::BeginPlay()
{
	Super::BeginPlay();

#if WITH_IMGUI
	SetupDebugTools();
#endif
}

void AStandardGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
#if WITH_IMGUI
	RemoveDebugTools();
#endif

	Super::EndPlay(EndPlayReason);
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

void AStandardGameMode::TransferCardBetweenPlayers_Internal(AStandardPlayerState* SrcPlayerState, AStandardPlayerState* DestPlayerState, int32 CardIndex)
{
	ACardBase* TransferCard = SrcPlayerState->GetCardByIndex(CardIndex);

	if (IsValid(TransferCard))
	{
		// Remove card from original owner
		SrcPlayerState->RemoveCard(CardIndex);

		TArray<FText> Arguments;
		Arguments.Add(TransferCard->GetCardDisplayInfo().GetCardNameText());
		SendGameMessageToPlayer(FGameMessage{TEXT("System"), EGameMessageType::GMT_System, TEXT("GameMsg_LoseCard"), Arguments}, SrcPlayerState->GetPlayerId());

		// Change card owner
		const int32 DestPlayerId = DestPlayerState->GetPlayerId();
		AController* NewOwnerController = GetPlayerControllerById(DestPlayerId);
		TransferCard->SetOwnerPlayerController(NewOwnerController ? NewOwnerController : GetAIControllerById(DestPlayerId));

		// Add card to new owner
		DestPlayerState->AddCard(TransferCard);

		SendGameMessageToPlayer(FGameMessage{TEXT("System"), EGameMessageType::GMT_System, TEXT("GameMsg_GainCard"), Arguments}, DestPlayerState->GetPlayerId());
	}
	else
	{
		UE_LOG(LogDogFight, Error, TEXT("Invalid card to transfer."));
	}
}

void AStandardGameMode::InitializeStateMachine()
{
#if WITH_IMGUI
	GameStartTime = FDateTime::Now();
#endif

	// Create state machine
	GameModeStateMachine = NewObject<UGameModeStateMachine>(this, UGameModeStateMachine::StaticClass(), TEXT("GameModeStateMachine"));
	GameModeStateMachine->SetOwnerGameMode(this);

	// Initialize all phases
	TArray<UGamePhase*> AllGamePhases;

	// Entering Map
	AllGamePhases.Add(NewObject<UStandardGameModePhaseBase>(GameModeStateMachine, UStandardGameModeEnteringMapPhase::StaticClass(), TEXT("GamePhase_EnteringMap")));
	AllGamePhases.Last()->InitializeGamePhase(StandardGameModePhase::EnteringMap, EGamePhaseType::GPT_Process, StandardGameModePhase::WaitingForStart);

	// Waiting for start
	UStandardGameModeTimedPhase* TimedPhase = NewObject<UStandardGameModeTimedPhase>(GameModeStateMachine, UStandardGameModeTimedPhase::StaticClass(), TEXT("GamePhase_WaitingForStart"));
	TimedPhase->InitializeGamePhase(StandardGameModePhase::WaitingForStart, EGamePhaseType::GPT_Process, StandardGameModePhase::SpawnPlayers);
	TimedPhase->SetPhaseDuration(GameStartDelay);
	AllGamePhases.Add(TimedPhase);

	// Spawn players
	AllGamePhases.Add(NewObject<UStandardGameModePhaseBase>(GameModeStateMachine, UStandardGameModeSpawnPlayersPhase::StaticClass(), TEXT("GamePhase_SpawnPlayers")));
	AllGamePhases.Last()->InitializeGamePhase(StandardGameModePhase::SpawnPlayers, EGamePhaseType::GPT_Process, StandardGameModePhase::FreeMoving);

	// Free moving
	UStandardGameModeFreeMovingPhase* FreeMovingPhase = NewObject<UStandardGameModeFreeMovingPhase>(GameModeStateMachine, UStandardGameModeFreeMovingPhase::StaticClass(),
		TEXT("GamePhase_FreeMoving"));
	FreeMovingPhase->InitializeGamePhase(StandardGameModePhase::FreeMoving, EGamePhaseType::GPT_Process, StandardGameModePhase::DecideOrder);
	FreeMovingPhase->SetPhaseDuration(FreeMovingDuration);
	AllGamePhases.Add(FreeMovingPhase);

	// Decide order
	AllGamePhases.Add(NewObject<UStandardGameModePhaseBase>(GameModeStateMachine, UStandardGameModeDecideOrderPhase::StaticClass(), TEXT("GamePhase_DecideOrder")));
	AllGamePhases.Last()->InitializeGamePhase(StandardGameModePhase::DecideOrder, EGamePhaseType::GPT_Process, StandardGameModePhase::SelectAbility);

	// Select ability
	AllGamePhases.Add(NewObject<UStandardGameModePhaseBase>(GameModeStateMachine, UStandardGameModeSelectAbilityPhase::StaticClass(), TEXT("GamePhase_SelectAbility")));
	AllGamePhases.Last()->InitializeGamePhase(StandardGameModePhase::SelectAbility, EGamePhaseType::GPT_Process, StandardGameModePhase::PlayerRoundBegin);

	// Player round begin
	UStandardGameModePlayerRoundBeginPhase* PlayerRoundBeginPhase = NewObject<UStandardGameModePlayerRoundBeginPhase>(GameModeStateMachine,
		UStandardGameModePlayerRoundBeginPhase::StaticClass(),	TEXT("GamePhase_PlayerRoundBegin"));
	PlayerRoundBeginPhase->InitializeGamePhase(StandardGameModePhase::PlayerRoundBegin, EGamePhaseType::GPT_Process, StandardGameModePhase::PlayerRound);
	PlayerRoundBeginPhase->SetPhaseAfterRequestFinish(StandardGameModePhase::PlayerRoundEnd);
	AllGamePhases.Add(PlayerRoundBeginPhase);

	// Player round
	UStandardGameModePlayerRoundPhase* PlayerRoundPhase = NewObject<UStandardGameModePlayerRoundPhase>(GameModeStateMachine, UStandardGameModePlayerRoundPhase::StaticClass(),
		TEXT("GamePhase_PlayerRound"));
	PlayerRoundPhase->InitializeGamePhase(StandardGameModePhase::PlayerRound, EGamePhaseType::GPT_Process, StandardGameModePhase::CharacterReturn);
	PlayerRoundPhase->SetPhaseAfterRequestFinish(StandardGameModePhase::CharacterReturn);
	AllGamePhases.Add(PlayerRoundPhase);

	// Character return
	AllGamePhases.Add(NewObject<UStandardGameModePhaseBase>(GameModeStateMachine, UStandardGameModeCharacterReturnPhase::StaticClass(), TEXT("GamePhase_CharacterReturn")));
	AllGamePhases.Last()->InitializeGamePhase(StandardGameModePhase::CharacterReturn, EGamePhaseType::GPT_Process, StandardGameModePhase::DiscardCards);

	// Discard cards
	AllGamePhases.Add(NewObject<UStandardGameModePhaseBase>(GameModeStateMachine, UStandardGameModeDiscardCardsPhase::StaticClass(), TEXT("GamePhase_DiscardCards")));
	AllGamePhases.Last()->InitializeGamePhase(StandardGameModePhase::DiscardCards, EGamePhaseType::GPT_Process, StandardGameModePhase::PlayerRoundEnd);

	// Player round end
	AllGamePhases.Add(NewObject<UStandardGameModePhaseBase>(GameModeStateMachine, UStandardGameModePlayerRoundEndPhase::StaticClass(), TEXT("GamePhase_PlayerRoundEnd")));
	AllGamePhases.Last()->InitializeGamePhase(StandardGameModePhase::PlayerRoundEnd, EGamePhaseType::GPT_Process, StandardGameModePhase::CheckGameEnd);

	// Check game end
	UStandardGameModeCheckGameEndPhase* CheckGameEndPhase = NewObject<UStandardGameModeCheckGameEndPhase>(GameModeStateMachine, UStandardGameModeCheckGameEndPhase::StaticClass(),
		TEXT("GamePhase_CheckGameEnd"));
	CheckGameEndPhase->InitializeGamePhase(StandardGameModePhase::CheckGameEnd, EGamePhaseType::GPT_Process, StandardGameModePhase::PlayerRoundBegin);
	CheckGameEndPhase->SetEndGamePhase(StandardGameModePhase::GameSummary);
	AllGamePhases.Add(CheckGameEndPhase);

	// Game summary
	AllGamePhases.Add(NewObject<UStandardGameModePhaseBase>(GameModeStateMachine, UStandardGameModeGameSummaryPhase::StaticClass(), TEXT("GamePhase_GameSummary")));
	AllGamePhases.Last()->InitializeGamePhase(StandardGameModePhase::GameSummary, EGamePhaseType::GPT_Process, NAME_None);

	GameModeStateMachine->OnGamePhaseChangedEvent.AddDynamic(this, &AStandardGameMode::OnGamePhaseChanged);

	GameModeStateMachine->RegisterGamePhase(AllGamePhases);
	GameModeStateMachine->StartWithPhase(StandardGameModePhase::EnteringMap);
}

void AStandardGameMode::OnGamePhaseChanged(FName NewPhase, uint8 SwitchMethod)
{
	// Sync game state
	if (AStandardGameState* StandardGameState = GetGameState<AStandardGameState>())
	{
		StandardGameState->SetCurrentGamePhase(NewPhase);
	}

#if WITH_IMGUI
	// Make a new record
	FDebugGamePhaseHistoryRecord NewRecord;
	NewRecord.PlayerId = GetCurrentPlayerId();
	NewRecord.GamePhaseName = NewPhase;
	const FTimespan TimeExpired = FDateTime::Now() - GameStartTime;
	if (TimeExpired.GetTicks() > 0)
	{
		NewRecord.TimeMinutes = FMath::Floor(TimeExpired.GetTotalMinutes());
		NewRecord.TimeSeconds = TimeExpired.GetSeconds();
	}
	NewRecord.SwitchMethod = SwitchMethod;
	StateMachineGamePhaseHistory.Add(NewRecord);
#endif
}

void AStandardGameMode::OnJoinedPlayerCountChanged()
{
	OnPlayerReadyForGame.Broadcast();
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

#if WITH_IMGUI
	// Update debug info
	if (PlayerBaseInfoMap.Contains(PlayerId))
	{
		FDebugPlayerBaseInfo* PlayerBaseInfo = PlayerBaseInfoMap.Find(PlayerId);
		PlayerBaseInfo->CurrentHealth = NewHealth;
	}
#endif
}

void AStandardGameMode::OnCharacterStrengthChangedCallback(int32 PlayerId, int32 NewStrength)
{
#if WITH_IMGUI
	// Update debug info
	if (PlayerBaseInfoMap.Contains(PlayerId))
	{
		FDebugPlayerBaseInfo* PlayerBaseInfo = PlayerBaseInfoMap.Find(PlayerId);
		PlayerBaseInfo->CurrentStrength = NewStrength;
	}
#endif
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
				//SetGamePhase(GamePhase::GameSummary);

				GameModeStateMachine->ForceJumpToPhase(StandardGameModePhase::GameSummary);
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

void AStandardGameMode::ToggleGameModeAdmin()
{
#if WITH_IMGUI
	bShowDebugTools = !bShowDebugTools;
	FImGuiModule::Get().GetProperties().ToggleInput();
#endif
}

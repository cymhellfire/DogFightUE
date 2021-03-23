// Dog Fight Game Code By CYM.


#include "Player/StandardModePlayerController.h"

#include "Pawns/StandardModePlayerPawn.h"
#include "Pawns/StandardModeCameraComponent.h"
#include "DogFight.h"
#include "Game/DogFightGameInstance.h"
#include "Game/SaveGameManager.h"
#include "Pawns/StandardModePlayerCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Game/DogFightSaveGame.h"
#include "UI/Widget/OperationHintMessageWidget.h"
#include "Game/StandardGameMode.h"
#include "UI/StandardHUD.h"
#include "Game/StandardPlayerState.h"
#include "UI/Widget/CardDisplayWidget.h"
#include "NavigationSystem.h"
#include "Actors/Managers/BuffQueue.h"
#include "Blueprint/UserWidget.h"

AStandardModePlayerController::AStandardModePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	bShowMouseCursor = true;
	InputMode = EStandardModePlayerControllerInputMode::IM_Disable;
	CharacterPawn = nullptr;
	PrimaryActorTick.bCanEverTick = true;
	bInGameMenuShown = false;
}

void AStandardModePlayerController::ClientSetClickMovementEnabled_Implementation(bool bEnabled)
{
	InputMode = bEnabled ? EStandardModePlayerControllerInputMode::IM_ClickMove : EStandardModePlayerControllerInputMode::IM_Disable;

	if (CharacterPawn != nullptr)
	{
		CharacterPawn->SetCursorVisible(bEnabled);
	}
#if !UE_BUILD_SHIPPING
	UE_LOG(LogDogFight, Log, TEXT("%s %s click movement."), *GetName(), (bEnabled ? TEXT("enabled") : TEXT("disabled")));
#endif
}

void AStandardModePlayerController::CloseInGameMenu()
{
	if (InGameMenuWidget && bInGameMenuShown)
	{
		InGameMenuWidget->RemoveFromParent();

		bInGameMenuShown = false;
	}
}

void AStandardModePlayerController::InitPlayerState()
{
	Super::InitPlayerState();

	// Register name changed delegate immediately on server side
	if (GetNetMode() != NM_Client)
	{
		if (AStandardPlayerState* StandardPlayerState = Cast<AStandardPlayerState>(PlayerState))
		{
			StandardPlayerState->OnPlayerNameChanged.AddDynamic(this, &AStandardModePlayerController::OnPlayerNameChanged);

			StandardPlayerState->OnPlayerCardInfoListChanged.AddDynamic(this, &AStandardModePlayerController::OnCardInfoListChanged);
			StandardPlayerState->OnPlayerCardUsingAbilityChanged.AddDynamic(this, &AStandardModePlayerController::OnCardUsingAbilityChanged);
			StandardPlayerState->OnPlayerCardUsableIndexChanged.AddDynamic(this, &AStandardModePlayerController::OnCardUsableIndexChanged);
		}
	}
}

void AStandardModePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (GetNetMode() == NM_Client)
	{
		if (AStandardPlayerState* StandardPlayerState = Cast<AStandardPlayerState>(PlayerState))
		{
			// Register name changed delegate on client side after PlayerState replicated
			StandardPlayerState->OnPlayerNameChanged.AddDynamic(this, &AStandardModePlayerController::OnPlayerNameChanged);

			// Acquire current player name
			if (!IsLocalController())
			{
				OnPlayerNameChanged(StandardPlayerState->GetPlayerName());
			}

			// Register card info list changed delegate
			StandardPlayerState->OnPlayerCardInfoListChanged.AddDynamic(this, &AStandardModePlayerController::OnCardInfoListChanged);
			StandardPlayerState->OnPlayerCardUsingAbilityChanged.AddDynamic(this, &AStandardModePlayerController::OnCardUsingAbilityChanged);
			StandardPlayerState->OnPlayerCardUsableIndexChanged.AddDynamic(this, &AStandardModePlayerController::OnCardUsableIndexChanged);
		}
	}
	
}

void AStandardModePlayerController::GameStart()
{
	// Spawn the character pawn once game started
	ServerSpawnCharacterPawn();
}

void AStandardModePlayerController::ClientReceivedGameTitleMessage_Implementation(FGameTitleMessage Message)
{
	if (AStandardHUD* StandardHUD = GetHUD<AStandardHUD>())
	{
		StandardHUD->ShowGameTitleMessage(Message);
	}
}

void AStandardModePlayerController::ServerBroadcastGameMessageToAll_Implementation(const FString& GameMessage, const TArray<FText>& Arguments)
{
	if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
	{
		FString SourceName;
		if (AStandardPlayerState* StandardPlayerState = GetPlayerState<AStandardPlayerState>())
		{
			SourceName = StandardPlayerState->GetPlayerName();
		}
		
		const FGameMessage NewMessage{SourceName, EGameMessageType::GMT_Player, GameMessage, Arguments};
		StandardGameMode->BroadcastGameMessageToAllPlayers(NewMessage);
	}
}

void AStandardModePlayerController::ClientReceivedGameMessage_Implementation(FGameMessage Message)
{
	if (AStandardHUD* StandardHUD = GetHUD<AStandardHUD>())
	{
		StandardHUD->ShowGameMessage(Message);
	}
}

void AStandardModePlayerController::ClientShowCardDisplayWidgetWithSelectMode_Implementation(ECardSelectionMode SelectionMode)
{
	if (AStandardHUD* StandardHUD = GetHUD<AStandardHUD>())
	{
		StandardHUD->ToggleCardDisplayWidgetVisibility(true);
		StandardHUD->SetCardSelectionMode(SelectionMode);
	}
}

void AStandardModePlayerController::OnCardSelectionConfirmed(TArray<int32>& SelectedIndexList)
{
	// Sort index descending for loop handling
	if (SelectedIndexList.Num() > 1)
	{
		SelectedIndexList.Sort([](const int32& A, const int32& B) { return A > B; });
	}

	ServerUploadSelectedCardIndex(SelectedIndexList);
}

APawn* AStandardModePlayerController::GetRandomPlayerPawn(bool bIgnoreSelf)
{
	AController* TargetController = nullptr;
	AGameModeBase* GameModeBase = GetWorld()->GetAuthGameMode();
	if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GameModeBase))
	{
		while(true)
		{
			TargetController = StandardGameMode->GetRandomController();
			if (bIgnoreSelf)
			{
				// Check if selected myself
				if (AStandardPlayerState* TargetPlayerState = TargetController->GetPlayerState<AStandardPlayerState>())
				{
					AStandardPlayerState* MyPlayerState = GetPlayerState<AStandardPlayerState>();
					if (MyPlayerState->GetPlayerId() != TargetPlayerState->GetPlayerId() && TargetPlayerState->IsAlive())
					{
						break;
					}
				}
			}
		}
	}

	if (IGameCardUserPlayerControllerInterface* PlayerControllerInterface = Cast<IGameCardUserPlayerControllerInterface>(TargetController))
	{
		return PlayerControllerInterface->GetActualPawn();
	}

	return nullptr;
}

void AStandardModePlayerController::OnCharacterDead()
{
	if (AStandardPlayerState* StandardPlayerState = GetPlayerState<AStandardPlayerState>())
	{
		StandardPlayerState->SetAlive(false);
		if (UBuffQueue* BuffQueue = StandardPlayerState->GetBuffQueue())
		{
			BuffQueue->StopCurrentProcessingBuff();
			BuffQueue->ClearQueue();
		}
		OnPlayerDead.Broadcast(StandardPlayerState->GetPlayerId());
	}
}

void AStandardModePlayerController::OnHealthChanged(int32 NewHealth)
{
	// Delegate
	if (APlayerState* MyPlayerState = GetPlayerState<APlayerState>())
	{
		OnPlayerHealthChanged.Broadcast(MyPlayerState->GetPlayerId(), NewHealth);
	}
}

void AStandardModePlayerController::ServerUploadSelectedCardIndex_Implementation(const TArray<int32>& SelectedIndexList)
{
	if (AStandardPlayerState* StandardPlayerState = GetPlayerState<AStandardPlayerState>())
	{
		if (SelectedIndexList.Num() > 0)
		{
			for (int32 Index : SelectedIndexList)
			{
				StandardPlayerState->ServerHandleSelectedCard(Index);
			}
		}
		else
		{
			StandardPlayerState->ServerHandleEmptySelectCard();
		}
	}
}

void AStandardModePlayerController::ClientHideCardDisplayWidget_Implementation()
{
	if (AStandardHUD* StandardHUD = GetHUD<AStandardHUD>())
	{
		StandardHUD->ToggleCardDisplayWidgetVisibility(false);
	}
}

void AStandardModePlayerController::ClientSetCardDisplayWidgetSelectable_Implementation(bool bSelectable)
{
	if (AStandardHUD* StandardHUD = GetHUD<AStandardHUD>())
	{
		StandardHUD->SetCardDisplayWidgetSelectable(bSelectable);

		if (bSelectable)
		{
			// Setup the delegate for get the selection result
			if (!StandardHUD->GetCardDisplayWidget()->OnCardSelectionConfirmed.IsAlreadyBound(this, &AStandardModePlayerController::OnCardSelectionConfirmed))
			{
				StandardHUD->GetCardDisplayWidget()->OnCardSelectionConfirmed.AddDynamic(this, &AStandardModePlayerController::OnCardSelectionConfirmed);
			}
		}
		else
		{
			// Clear the delegate
			if (StandardHUD->GetCardDisplayWidget()->OnCardSelectionConfirmed.IsAlreadyBound(this, &AStandardModePlayerController::OnCardSelectionConfirmed))
			{
				StandardHUD->GetCardDisplayWidget()->OnCardSelectionConfirmed.RemoveDynamic(this, &AStandardModePlayerController::OnCardSelectionConfirmed);
			}
		}
	}
}

void AStandardModePlayerController::ClientStartDiscardCards_Implementation(int32 CountToDiscard)
{
	if (AStandardHUD* StandardHUD = GetHUD<AStandardHUD>())
	{
		StandardHUD->StartDiscardCards(CountToDiscard);
		StandardHUD->DisplayHintMessage(EOperationHintMessageContent::DiscardCard);
	}
}

void AStandardModePlayerController::ClientStopDiscardCards_Implementation()
{
	if (AStandardHUD* StandardHUD = GetHUD<AStandardHUD>())
	{
		StandardHUD->StopDiscardCards();
		StandardHUD->HideOperationHintMessageWidget();
	}
}

void AStandardModePlayerController::StopCharacterMovementImmediately()
{
	if (GetNetMode() == NM_Client)
		return;

	if (CharacterPawn != nullptr)
	{
		CharacterPawn->StopMoveImmediately();
	}
}

void AStandardModePlayerController::ClientRequestCardByClasses_Implementation(const TArray<TSubclassOf<ACardBase>>& RequestCardClasses)
{

}

void AStandardModePlayerController::RequestActorTarget()
{
	// Only server can dispatch the request
	if (GetNetMode() == NM_Client)
		return;

	ClientSelectActorTarget();
}

void AStandardModePlayerController::RequestPositionTarget()
{
	if (GetNetMode() == NM_Client)
		return;

	ClientSelectPositionTarget();
}

void AStandardModePlayerController::RequestDirectionTarget()
{
	if (GetNetMode() == NM_Client)
		return;

	ClientSelectDirectionTarget();
}

FCardInstructionTargetInfo AStandardModePlayerController::RequestRandomActorTarget(bool bIgnoreSelf)
{
	// Make TargetInfo
	FCardInstructionTargetInfo ResultInfo;
	ResultInfo.ActorPtr = GetRandomPlayerPawn(bIgnoreSelf);
	ResultInfo.TargetType = ECardInstructionTargetType::Actor;

	return ResultInfo;
}

FCardInstructionTargetInfo AStandardModePlayerController::RequestRandomPositionTarget()
{
	FVector TargetPosition(0, 0, 0);
	FNavLocation NavLocation;
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys && NavSys->GetRandomPoint(NavLocation))
	{
		TargetPosition = NavLocation.Location;
	}

	// Make TargetInfo
	FCardInstructionTargetInfo ResultInfo;
	ResultInfo.ActorPtr = nullptr;
	ResultInfo.PositionValue = TargetPosition;
	ResultInfo.TargetType = ECardInstructionTargetType::Position;
	
	return ResultInfo;
}

FCardInstructionTargetInfo AStandardModePlayerController::RequestRandomDirectionTarget()
{
	// Make TargetInfo
	const float RandomAngle = FMath::RandRange(-3.1415926f, 3.1415926f);

	FCardInstructionTargetInfo ResultInfo;
	ResultInfo.ActorPtr = nullptr;
	ResultInfo.DirectionValue = FVector(FMath::Sin(RandomAngle), FMath::Cos(RandomAngle), 0);
	ResultInfo.TargetType = ECardInstructionTargetType::Direction;

	return ResultInfo;
}

APawn* AStandardModePlayerController::GetActualPawn()
{
	return Cast<APawn>(CharacterPawn);
}

void AStandardModePlayerController::BroadcastCardTargetingResult(FText CardName, FText TargetText, ECardInstructionTargetType TargetType)
{
	if (GetNetMode() == NM_Client)
	{
		return;
	}

	if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
	{
		FGameMessage NewMessage;
		NewMessage.Type = EGameMessageType::GMT_Player;
		NewMessage.Source = PlayerState->GetPlayerName();
		switch(TargetType)
		{
		case ECardInstructionTargetType::Actor:
			NewMessage.MessageString = TEXT("GameMsg_UseCard_Actor");
			break;
		case ECardInstructionTargetType::Position:
			NewMessage.MessageString = TEXT("GameMsg_UseCard_Position");
			break;
		case ECardInstructionTargetType::Direction:
			NewMessage.MessageString = TEXT("GameMsg_UseCard_Direction");
			break;
		default: ;
		}
		NewMessage.Arguments = {CardName, TargetText};
		StandardGameMode->BroadcastGameMessageToAllPlayers(NewMessage);
	}
}

void AStandardModePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Create the InGameMenu widget for local player
	if (IsLocalController())
	{
		FStringClassReference WidgetClassRef(TEXT("/Game/DogFight/UI/InGame/BP_InGameMenu.BP_InGameMenu_C"));
		if (UClass* WidgetClass = WidgetClassRef.TryLoadClass<UUserWidget>())
		{
			InGameMenuWidget = CreateWidget<UUserWidget>(this, WidgetClass);
		}
		else
		{
			UE_LOG(LogLoad, Error, TEXT("Failed to load class %s"), *WidgetClassRef.ToString());
		}

		// Update ready state
		if (GetNetMode() == NM_Client)
		{
			UDogFightSaveGame* SaveGameInstance = Cast<UDogFightGameInstance>(GetGameInstance())->GetSaveGameManager()->GetCurrentSaveGameInstance();
			if (SaveGameInstance == nullptr)
			{
				UE_LOG(LogDogFight, Error, TEXT("No available player profile."));
				return;
			}
			
			ServerReadyForGame(SaveGameInstance->PlayerName);
		}
		// Host register to Timeline here
		else
		{
			ServerRegisterToGameTimeline();
		}
	}
}

void AStandardModePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AStandardModePlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("OpenInGameMenu", IE_Pressed, this, &AStandardModePlayerController::OnOpenInGameMenuPressed);
}

void AStandardModePlayerController::ProcessPlayerInput(const float DeltaTime, const bool bGamePaused)
{
	// Don't update camera movement when in game menu displaying
	if (bInGameMenuShown)
		return;

	Super::ProcessPlayerInput(DeltaTime, bGamePaused);

	// Update StandardModePlayerPawn if available
	AStandardModePlayerPawn* CurrentPlayerPawn = Cast<AStandardModePlayerPawn>(GetPawn());
	if (CurrentPlayerPawn != nullptr)
	{
		CurrentPlayerPawn->GetStandardModeCameraComponent()->UpdateCameraMovement(this);
	}
}

void AStandardModePlayerController::GatherPlayerInfo()
{
	// Get player name
	UDogFightSaveGame* SaveGameInstance = Cast<UDogFightGameInstance>(GetGameInstance())->GetSaveGameManager()->GetCurrentSaveGameInstance();
	if (SaveGameInstance == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("No available player profile."));
		return;
	}

	// Set player name
	ServerChangeName(SaveGameInstance->PlayerName);
}

void AStandardModePlayerController::DisableInputMode()
{
	if (InputMode == EStandardModePlayerControllerInputMode::IM_Disable)
		return;

	InputMode = EStandardModePlayerControllerInputMode::IM_Disable;

	// Clear the operation hint
	if (AStandardHUD* StandardHUD = GetHUD<AStandardHUD>())
	{
		StandardHUD->HideOperationHintMessageWidget();
	}
}

void AStandardModePlayerController::OnPlayerNameChanged(const FString& NewName)
{
	if (CharacterPawn != nullptr)
	{
		CharacterPawn->SetUnitName(NewName);
	}
	else
	{
		// Set the new name to buffer
		PendingUnitName = NewName;
	}

	// Clients register to Timeline after they uploaded name
	ServerRegisterToGameTimeline();
}

void AStandardModePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStandardModePlayerController, CharacterPawnClass);
	DOREPLIFETIME(AStandardModePlayerController, CharacterPawn);
}

void AStandardModePlayerController::OnSetDestinationPressed()
{
	if (bInGameMenuShown)
		return;

	if (InputMode == EStandardModePlayerControllerInputMode::IM_Disable)
		return;

	FHitResult HitResult;

	switch(InputMode)
	{
	case EStandardModePlayerControllerInputMode::IM_ClickMove:
		// Check the pawn is spawned
		if (CharacterPawn == nullptr)
			return;

		GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		if (HitResult.bBlockingHit)
		{
			float const Distance = FVector::Dist(HitResult.Location, CharacterPawn->GetActorLocation());
			if (Distance > 120.f)
			{
				// Call RPC function on server side
				ServerMoveToMouseCursor(HitResult.Location);
			}
		}
		break;
	case EStandardModePlayerControllerInputMode::IM_TargetingActor:
		/** Use customized trace channel 'SelectActor' here. */
		GetHitResultUnderCursor(ECC_GameTraceChannel1, false, HitResult);

		// Get the actor clicked
		if (HitResult.bBlockingHit)
		{
			if (AActor* TargetActor = HitResult.GetActor())
			{
				ServerSyncActorTarget(TargetActor);

				// Disable selection after acquire one
				DisableInputMode();
			}
		}
		break;
	case EStandardModePlayerControllerInputMode::IM_TargetingPosition:
		GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		// Get the point clicked
		if (HitResult.bBlockingHit)
		{
			ServerSyncPositionTarget(HitResult.Location);

			// Disable selection after acquire one
			DisableInputMode();
		}
		break;
	case EStandardModePlayerControllerInputMode::IM_TargetingDirection:
		// Check the pawn is spawned
		if (CharacterPawn == nullptr)
			return;

		GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		// Calculate the direction
		if (HitResult.bBlockingHit)
		{
			FVector TargetDirection = HitResult.Location - CharacterPawn->GetActorLocation();
			TargetDirection.Normalize();

			ServerSyncDirectionTarget(TargetDirection);

			// Disable selection after acquire one
			DisableInputMode();
		}
		break;
	case EStandardModePlayerControllerInputMode::IM_Disable:
	default:
		/* Do nothing as default. */;
	}
}

void AStandardModePlayerController::OnOpenInGameMenuPressed()
{
	if (InGameMenuWidget && !bInGameMenuShown)
	{
		InGameMenuWidget->AddToViewport();

		bInGameMenuShown = true;
	}
}

void AStandardModePlayerController::ServerSetCharacterName_Implementation(const FString& NewName)
{
	if (CharacterPawn != nullptr)
	{
		CharacterPawn->SetUnitName(NewName);
	}
}

void AStandardModePlayerController::ServerReadyForGame_Implementation(const FString& PlayerName)
{
	if (AStandardGameMode* GameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->PlayerReadyForGame(PlayerName);
	}
}

void AStandardModePlayerController::ServerSetCharacterHealth_Implementation(int32 NewHealth)
{
	if (CharacterPawn != nullptr)
	{
		CharacterPawn->SetCurrentHealth(NewHealth);
	}
}

void AStandardModePlayerController::ServerRegisterToGameTimeline_Implementation()
{
	if (AStandardPlayerState* StandardPlayerState = GetPlayerState<AStandardPlayerState>())
	{
		if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
		{
			StandardGameMode->RegisterPlayerToTimeline(this);
		}
	}
}

void AStandardModePlayerController::ClientSetupTimelineDisplay_Implementation()
{
	if (AStandardHUD* StandardHUD = GetHUD<AStandardHUD>())
	{
		StandardHUD->InitializeTimelineDisplayTimeline();

		// Show the Timeline widget after initialized
		StandardHUD->SetTimelineVisibility(true);
	}
}

void AStandardModePlayerController::ServerFinishMyRound_Implementation()
{
	if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (AStandardPlayerState* StandardPlayerState = GetPlayerState<AStandardPlayerState>())
		{
			if (StandardGameMode->GetCurrentPlayerId() == StandardPlayerState->GetPlayerId())
			{
				StandardGameMode->EndCurrentPlayerRound();
			}
		}
	}
}

void AStandardModePlayerController::OnRep_CharacterPawn()
{
	// Set the buffed name to new spawned character
	if (!PendingUnitName.IsEmpty())
	{
		ServerSetCharacterName(PendingUnitName);
		// Clear buffer after use
		PendingUnitName = "";
	}

	// Update decal visibility
	CharacterPawn->SetCursorVisible(InputMode == EStandardModePlayerControllerInputMode::IM_ClickMove);
}

void AStandardModePlayerController::OnCardInfoListChanged()
{
	if (AStandardHUD* StandardHUD = GetHUD<AStandardHUD>())
	{
		if (AStandardPlayerState* StandardPlayerState = GetPlayerState<AStandardPlayerState>())
		{
			StandardHUD->SetCardDisplayInfoList(StandardPlayerState->GetCardDisplayInfoList());
		}
	}
}

void AStandardModePlayerController::OnCardUsingAbilityChanged()
{
	if (AStandardHUD* StandardHUD = GetHUD<AStandardHUD>())
	{
		StandardHUD->RefreshCardUsingAbilityInfo();
	}
}

void AStandardModePlayerController::OnCardUsableIndexChanged()
{
	if (AStandardHUD* StandardHUD = GetHUD<AStandardHUD>())
	{
		StandardHUD->RefreshCardUsableState();
	}
}

void AStandardModePlayerController::ServerSyncDirectionTarget_Implementation(FVector TargetDirection)
{
	// Package the target info struct
	FCardInstructionTargetInfo NewTargetInfo;
	NewTargetInfo.DirectionValue = TargetDirection;
	NewTargetInfo.TargetType = ECardInstructionTargetType::Direction;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Selected direction %s"), *TargetDirection.ToString()));

	OnCardTargetInfoAcquired.Broadcast(NewTargetInfo);

	// Let the character aim at target
	CharacterPawn->SetAimingDirection(TargetDirection);
}

void AStandardModePlayerController::ClientSelectDirectionTarget_Implementation()
{
	// Setup direction select mode
	InputMode = EStandardModePlayerControllerInputMode::IM_TargetingDirection;

	// Update the operation hint
	if (AStandardHUD* StandardHUD = GetHUD<AStandardHUD>())
	{
		StandardHUD->DisplayHintMessage(EOperationHintMessageContent::SelectDirectionTarget);
	}
}

void AStandardModePlayerController::ServerSyncPositionTarget_Implementation(FVector TargetPosition)
{
	// Package the target info struct
	FCardInstructionTargetInfo NewTargetInfo;
	NewTargetInfo.PositionValue = TargetPosition;
	NewTargetInfo.TargetType = ECardInstructionTargetType::Position;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Selected position %s"), *TargetPosition.ToString()));

	OnCardTargetInfoAcquired.Broadcast(NewTargetInfo);

	// Let the character aim at target
	const FVector AimingDirection = TargetPosition - CharacterPawn->GetActorLocation();
	if (AimingDirection.Size() > 0.01f)
	{
		CharacterPawn->SetAimingDirection(AimingDirection);
	}
}

void AStandardModePlayerController::ClientSelectPositionTarget_Implementation()
{
	// Setup position select mode
	InputMode = EStandardModePlayerControllerInputMode::IM_TargetingPosition;

	// Update the operation hint
	if (AStandardHUD* StandardHUD = GetHUD<AStandardHUD>())
	{
		StandardHUD->DisplayHintMessage(EOperationHintMessageContent::SelectPositionTarget);
	}
}

void AStandardModePlayerController::ServerSyncActorTarget_Implementation(AActor* TargetActor)
{
	// Trigger delegate before create new target info to gain possibility to change target now
	AActor** TargetActorAddress = &TargetActor;
	OnTargetActorSelected.Broadcast(TargetActorAddress);

	// Override target actor if changed
	if (*TargetActorAddress != TargetActor)
	{
		TargetActor = *TargetActorAddress;
	}

	// Package the target info struct
	FCardInstructionTargetInfo NewTargetInfo;
	NewTargetInfo.ActorPtr = TargetActor;
	NewTargetInfo.TargetType = ECardInstructionTargetType::Actor;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Selected %s"), *TargetActor->GetName()));

	OnCardTargetInfoAcquired.Broadcast(NewTargetInfo);

	// Let the character aim at target
	const FVector AimingDirection = TargetActor->GetActorLocation() - CharacterPawn->GetActorLocation();
	if (AimingDirection.Size() > 0.01f)
	{
		CharacterPawn->SetAimingDirection(AimingDirection);
	}
}

void AStandardModePlayerController::ClientSelectActorTarget_Implementation()
{
	// Setup actor select mode
	InputMode = EStandardModePlayerControllerInputMode::IM_TargetingActor;

	// Update the operation hint
	if (AStandardHUD* StandardHUD = GetHUD<AStandardHUD>())
	{
		StandardHUD->DisplayHintMessage(EOperationHintMessageContent::SelectActorTarget);
	}
}

void AStandardModePlayerController::ExecSetCharacterName(FString NewName)
{
	if (IsLocalController())
	{
		ServerSetCharacterName(NewName);
	}
}

void AStandardModePlayerController::ExecSetCurrentHealth(int32 NewHealth)
{
	if (IsLocalController())
	{
		ServerSetCharacterHealth(NewHealth);
	}
}

void AStandardModePlayerController::ExecRequireActorTarget()
{
	if (GetNetMode() != NM_Client)
	{
		ClientSelectActorTarget();
	}
	else
	{
		ClientSelectActorTarget_Implementation();
	}
}

void AStandardModePlayerController::ExecRequirePositionTarget()
{
	if (GetNetMode() != NM_Client)
	{
		ClientSelectPositionTarget();
	}
	else
	{
		ClientSelectPositionTarget_Implementation();
	}
}

void AStandardModePlayerController::ExecRequireDirectionTarget()
{
	if (GetNetMode() != NM_Client)
	{
		ClientSelectDirectionTarget();
	}
	else
	{
		ClientSelectDirectionTarget_Implementation();
	}
}

void AStandardModePlayerController::ExecToggleCardDisplayWidget(bool bVisible)
{
	if (AStandardHUD* StandardHUD = GetHUD<AStandardHUD>())
	{
		StandardHUD->ToggleCardDisplayWidgetVisibility(bVisible);
	}
}

void AStandardModePlayerController::ExecSetRagdoll(bool bActive)
{
	if (CharacterPawn == nullptr)
		return;

	CharacterPawn->MulticastSetRagdollActive(bActive);
}

void AStandardModePlayerController::ExecEquipTestWeapon()
{
	if (CharacterPawn == nullptr)
		return;

	CharacterPawn->EquipTestWeapon();
}

void AStandardModePlayerController::ExecUnEquipTestWeapon()
{
	if (CharacterPawn == nullptr)
		return;

	CharacterPawn->UnEquipWeapon();
}

void AStandardModePlayerController::ExecEnqueueInput(uint8 InputIndex, int32 InputCount)
{
	if (CharacterPawn == nullptr)
		return;

	for (int32 i = 0; i < InputCount; ++i)
	{
		CharacterPawn->EnqueueInput(static_cast<EWeaponActionInput>(InputIndex));
	}
}

void AStandardModePlayerController::ServerMoveToMouseCursor_Implementation(FVector Destination)
{
	if (CharacterPawn == nullptr)
		return;

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(CharacterPawn->GetController(), Destination);
}

void AStandardModePlayerController::ServerSpawnCharacterPawn_Implementation()
{
	// Only do spawn on server side
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	UE_LOG(LogDogFight, Display, TEXT("%s Request Spawn Character Pawn."), *GetName());
	
	// Spawn character pawn now
	if (CharacterPawn == nullptr)
	{
		if (CharacterPawnClass != nullptr)
		{
			// Get a random PlayerStart as spawn location
			TArray<AActor*> StartPoints;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), StartPoints);
			const FVector StartPoint = StartPoints.Num() > 0 ? StartPoints[FMath::RandRange(0, StartPoints.Num() - 1)]->GetActorLocation() : FVector::ZeroVector;

			CharacterPawn = GetWorld()->SpawnActor<AStandardModePlayerCharacter>(
				CharacterPawnClass->GetDefaultObject()->GetClass(), StartPoint,
				RootComponent->GetComponentRotation());
			CharacterPawn->SetOwner(this);
			CharacterPawn->SetPlayerState(PlayerState);
			CharacterPawn->SetSupremeController(this);
			// Register listener
			CharacterPawn->OnCharacterDead.AddDynamic(this, &AStandardModePlayerController::OnCharacterDead);
			CharacterPawn->OnCharacterHealthChanged.AddDynamic(this, &AStandardModePlayerController::OnHealthChanged);
			UE_LOG(LogDogFight, Display, TEXT("Spawn location at %s"), *(StartPoint.ToString()));
		}
		else
		{
			UE_LOG(LogDogFight, Error, TEXT("No pawn has been specified as character."));
		}
	}

	// Trigger OnRep_CharacterPawn on server manually
	OnRep_CharacterPawn();
}




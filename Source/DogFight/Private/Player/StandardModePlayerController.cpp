// Dog Fight Game Code By CYM.


#include "StandardModePlayerController.h"

#include "StandardModePlayerPawn.h"
#include "StandardModeCameraComponent.h"
#include "DogFight.h"
#include "DogFightGameInstance.h"
#include "SaveGameManager.h"
#include "StandardModePlayerCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Net/UnrealNetwork.h"
#include "DogFightSaveGame.h"
#include "StandardGameMode.h"
#include "StandardPlayerState.h"
#include "Blueprint/UserWidget.h"

AStandardModePlayerController::AStandardModePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	bShowMouseCursor = true;
	bClickMoveEnabled = true;
	CharacterPawn = nullptr;
	PrimaryActorTick.bCanEverTick = true;
	bInGameMenuShown = false;
}

void AStandardModePlayerController::SetClickMovementEnabled(bool bEnabled)
{
	bClickMoveEnabled = bEnabled;
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
		}
	}
}

void AStandardModePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Register name changed delegate on client side after PlayerState replicated
	if (GetNetMode() == NM_Client)
	{
		if (AStandardPlayerState* StandardPlayerState = Cast<AStandardPlayerState>(PlayerState))
		{
			StandardPlayerState->OnPlayerNameChanged.AddDynamic(this, &AStandardModePlayerController::OnPlayerNameChanged);

			// Acquire current player name
			if (!IsLocalController())
			{
				OnPlayerNameChanged(StandardPlayerState->GetPlayerName());
			}
		}
	}
}

void AStandardModePlayerController::GameStart()
{
	// Spawn the character pawn once game started
	CmdSpawnCharacterPawn();
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
			CmdReadyForGame();
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
}

void AStandardModePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStandardModePlayerController, CharacterPawnClass);
	DOREPLIFETIME(AStandardModePlayerController, CharacterPawn);
	DOREPLIFETIME(AStandardModePlayerController, bClickMoveEnabled);
}

void AStandardModePlayerController::OnSetDestinationPressed()
{
	if (bInGameMenuShown)
		return;

	// Check if click movement is enabled
	if (!bClickMoveEnabled || CharacterPawn == nullptr)
		return;
	
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	if (HitResult.bBlockingHit)
	{
		float const Distance = FVector::Dist(HitResult.Location, CharacterPawn->GetActorLocation());
		if (Distance > 120.f)
		{
			// Call RPC function on server side
			CmdMoveToMouseCursor(HitResult.Location);
		}
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

void AStandardModePlayerController::CmdSetCharacterName_Implementation(const FString& NewName)
{
	if (CharacterPawn != nullptr)
	{
		CharacterPawn->SetUnitName(NewName);
	}
}

void AStandardModePlayerController::CmdReadyForGame_Implementation()
{
	if (AStandardGameMode* GameMode = Cast<AStandardGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->PlayerReadyForGame();
	}
}

void AStandardModePlayerController::OnRep_CharacterPawn()
{
	// Set the buffed name to new spawned character
	if (!PendingUnitName.IsEmpty())
	{
		CmdSetCharacterName(PendingUnitName);
		// Clear buffer after use
		PendingUnitName = "";
	}
}

void AStandardModePlayerController::ExecSetCharacterName(FString NewName)
{
	if (IsLocalController())
	{
		CmdSetCharacterName(NewName);
	}
}

void AStandardModePlayerController::CmdMoveToMouseCursor_Implementation(FVector Destination)
{
	if (CharacterPawn == nullptr)
		return;

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(CharacterPawn->GetController(), Destination);
}

void AStandardModePlayerController::CmdSpawnCharacterPawn_Implementation()
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




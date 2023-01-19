#include "PlayerController/TopDownStylePlayerController.h"

#include "Card/Card.h"
#include "GameMode/TopDownStyleGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Pawn/PlayerCharacter/FreeForAllPlayerCharacter.h"
#include "Player/TopDownStylePlayerState.h"
#include "Player/ControllerComponent/CardTargetProviderComponent.h"
#include "PlayerController/PlayerControllerComponent/InGameMessageReceiverComponent.h"

ATopDownStylePlayerController::ATopDownStylePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create in-game message receiver
	InGameMessageReceiverComponent = CreateDefaultSubobject<UInGameMessageReceiverComponent>("InGameMessageReceiver");
	// Create target acquire component
	CardTargetProviderComponent = CreateDefaultSubobject<UCardTargetProviderComponent>("CardTargetProvider");
	CardTargetProviderComponent->OnCardTargetAcquired.AddDynamic(this, &ATopDownStylePlayerController::OnCardTargetAcquired);

	bShowMouseCursor = true;
}

void ATopDownStylePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		// ServerMarkPlayerReady();
		GetWorld()->GetTimerManager().SetTimer(RandomTimer, this, &ATopDownStylePlayerController::OnTimerExpired,
			FMath::RandRange(2, 5));
	}
}

void ATopDownStylePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedPtr;
	SharedPtr.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ATopDownStylePlayerController, CharacterPawn, SharedPtr);
}

void ATopDownStylePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	CardTargetProviderComponent->InitializeInput();
}

void ATopDownStylePlayerController::SpawnCharacterPawn()
{
	if (HasAuthority())
	{
		if (CharacterPawn == nullptr)
		{
			MARK_PROPERTY_DIRTY_FROM_NAME(ATopDownStylePlayerController, CharacterPawn, this);

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			CharacterPawn = GetWorld()->SpawnActor<AFreeForAllPlayerCharacter>(CharacterClass, FVector::ZeroVector,
				FRotator::ZeroRotator, SpawnParameters);
		}
	}
}

void ATopDownStylePlayerController::ServerMarkPlayerReady_Implementation()
{
	if (ATopDownStyleGameMode* GameMode = Cast<ATopDownStyleGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->PlayerReadyForGame(this);
	}
}

void ATopDownStylePlayerController::OnTimerExpired()
{
	if (RandomTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(RandomTimer);
	}

	ServerMarkPlayerReady();
}

void ATopDownStylePlayerController::ServerUseCardByInstanceId_Implementation(int32 InId)
{
	if (auto PS = GetPlayerState<ATopDownStylePlayerState>())
	{
		// Let player state start the using process
		PS->ServerTryToUseCardByInstanceId(InId);
	}
}

void ATopDownStylePlayerController::StartAcquireTargets(FTargetAcquireSettings Settings, TFunction<void(bool bSuccess, TArray<FAcquiredTargetInfo>)> Callback)
{
	AcquireTargetCallback = Callback;

	CardTargetProviderComponent->AcquireTarget(Settings);
}

void ATopDownStylePlayerController::OnCardTargetAcquired(bool bSuccess)
{
	if (AcquireTargetCallback)
	{
		if (bSuccess)
		{
			AcquireTargetCallback(bSuccess, CardTargetProviderComponent->GetLastTargetInfoList());
		}
		else
		{
			AcquireTargetCallback(bSuccess, TArray<FAcquiredTargetInfo>());
		}

		// The callback should be clear once invoked
		AcquireTargetCallback.Reset();
	}
}

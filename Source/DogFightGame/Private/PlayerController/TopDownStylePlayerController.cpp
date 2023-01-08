#include "PlayerController/TopDownStylePlayerController.h"

#include "GameMode/TopDownStyleGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Pawn/PlayerCharacter/FreeForAllPlayerCharacter.h"
#include "PlayerController/PlayerControllerComponent/InGameMessageReceiverComponent.h"

ATopDownStylePlayerController::ATopDownStylePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create in-game message receiver
	InGameMessageReceiverComponent = CreateDefaultSubobject<UInGameMessageReceiverComponent>("InGameMessageReceiver");

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

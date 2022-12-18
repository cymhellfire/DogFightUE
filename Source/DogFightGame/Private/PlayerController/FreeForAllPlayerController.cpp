#include "PlayerController/FreeForAllPlayerController.h"

#include "GameMode/FreeForAllGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Pawn/PlayerCharacter/FreeForAllPlayerCharacter.h"
#include "PlayerController/PlayerControllerComponent/InGameMessageReceiverComponent.h"

AFreeForAllPlayerController::AFreeForAllPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create in-game message receiver
	InGameMessageReceiverComponent = CreateDefaultSubobject<UInGameMessageReceiverComponent>("InGameMessageReceiver");
}

void AFreeForAllPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		// ServerMarkPlayerReady();
		GetWorld()->GetTimerManager().SetTimer(RandomTimer, this, &AFreeForAllPlayerController::OnTimerExpired,
			FMath::RandRange(2, 5));
	}
}

void AFreeForAllPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedPtr;
	SharedPtr.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(AFreeForAllPlayerController, CharacterPawn, SharedPtr);
}

void AFreeForAllPlayerController::SpawnCharacterPawn()
{
	if (HasAuthority())
	{
		if (CharacterPawn == nullptr)
		{
			MARK_PROPERTY_DIRTY_FROM_NAME(AFreeForAllPlayerController, CharacterPawn, this);

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			CharacterPawn = GetWorld()->SpawnActor<AFreeForAllPlayerCharacter>(CharacterClass, FVector::ZeroVector,
				FRotator::ZeroRotator, SpawnParameters);
		}
	}
}

void AFreeForAllPlayerController::ServerMarkPlayerReady_Implementation()
{
	if (AFreeForAllGameMode* GameMode = Cast<AFreeForAllGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->PlayerReadyForGame(this);
	}
}

void AFreeForAllPlayerController::OnTimerExpired()
{
	if (RandomTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(RandomTimer);
	}

	ServerMarkPlayerReady();
}

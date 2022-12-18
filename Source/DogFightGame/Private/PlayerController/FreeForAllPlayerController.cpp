#include "PlayerController/FreeForAllPlayerController.h"

#include "GameMode/FreeForAllGameMode.h"
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

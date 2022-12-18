#include "PlayerController/FreeForAllPlayerController.h"

#include "GameMode/FreeForAllGameMode.h"

void AFreeForAllPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		ServerMarkPlayerReady();
		// GetWorld()->GetTimerManager().SetTimer(RandomTimer, this, &AFreeForAllPlayerController::OnTimerExpired,
		// 	FMath::RandRange(2, 5));
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

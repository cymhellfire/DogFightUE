// Dog Fight Game Code By CYM.


#include "StandardGameMode.h"

#include "DogFight.h"
#include "StandardModePlayerController.h"


void AStandardGameMode::EnablePlayerClickMovement()
{
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStandardModePlayerController::StaticClass(), ActorList);

	for(AActor* Actor : ActorList)
	{
		AStandardModePlayerController* PlayerController = Cast<AStandardModePlayerController>(Actor);
		if (PlayerController != nullptr)
		{
			PlayerController->SetClickMovementEnabled(true);
		}
	}
}

void AStandardGameMode::DisablePlayerClickMovement()
{
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStandardModePlayerController::StaticClass(), ActorList);

	for(AActor* Actor : ActorList)
	{
		AStandardModePlayerController* PlayerController = Cast<AStandardModePlayerController>(Actor);
		if (PlayerController != nullptr)
		{
			PlayerController->SetClickMovementEnabled(false);
		}
	}
}

APlayerController* AStandardGameMode::SpawnPlayerController(ENetRole InRemoteRole, const FString& Options)
{
	APlayerController* PlayerController = Super::SpawnPlayerController(InRemoteRole, Options);

	// Cast and record AStandardModePlayerController
	AStandardModePlayerController* StandardModePlayerController = Cast<AStandardModePlayerController>(PlayerController);
	if (StandardModePlayerController != nullptr)
	{
		if (PlayerControllerList.Contains(StandardModePlayerController) == false)
		{
			PlayerControllerList.Add(StandardModePlayerController);

#if !UE_BUILD_SHIPPING
			UE_LOG(LogDogFight, Log, TEXT("Add controller [%s] to list."), *StandardModePlayerController->GetName());
#endif
		}
	}

	return PlayerController;
}

void AStandardGameMode::Logout(AController* Exiting)
{
	// Check if this Controller is recorded
	AStandardModePlayerController* StandardModePlayerController = Cast<AStandardModePlayerController>(Exiting);
	if (StandardModePlayerController != nullptr)
	{
		if (PlayerControllerList.Contains(StandardModePlayerController))
		{
			PlayerControllerList.Remove(StandardModePlayerController);
			
#if !UE_BUILD_SHIPPING
			UE_LOG(LogDogFight, Log, TEXT("Remove controller [%s] from list."), *StandardModePlayerController->GetName());
#endif
		}
	}

	// Continue to do other things
	Super::Logout(Exiting);
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

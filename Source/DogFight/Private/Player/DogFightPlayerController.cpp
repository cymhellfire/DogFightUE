// Dog Fight Game Code By CYM.


#include "Player/DogFightPlayerController.h"

#include "Game/DogFightGameInstance.h"
#include "Common/Localization.h"

void ADogFightPlayerController::ClientReturnToMainMenuWithReason_Implementation(EReturnToMainMenuReason::Type Reason)
{
	if (UGameInstance* const GameInstance = GetGameInstance())
	{
		// Remove the error handler first
		if (UDogFightGameInstance* DogFightGameInstance = Cast<UDogFightGameInstance>(GameInstance))
		{
			DogFightGameInstance->RemoveNetworkFailureHandler();
		}

		GameInstance->ReturnToMainMenu();

		// Add reason message
		if (UDogFightGameInstance* DogFightGameInstance = Cast<UDogFightGameInstance>(GameInstance))
		{
#if UE_EDITOR
			const FString ErrorContextString = UEnum::GetDisplayValueAsText(Reason).ToString();
#else
			const FString ErrorContextString = FString::Printf(TEXT("NetError_%s"),*UEnum::GetDisplayValueAsText(Reason).ToString());
#endif
			UE_LOG(LogDogFight, Log, TEXT("DisplayValueAsText: %s"), *ErrorContextString);
			
			DogFightGameInstance->AddPendingMessage(FText::FromStringTable(ST_UI_LOC, TEXT("NetErrorTitle")),
				FText::FromStringTable(ST_UI_LOC, ErrorContextString));
		}
	}
	else
	{
		UWorld* const World = GetWorld();
		GEngine->HandleDisconnect(World, World->GetNetDriver());
	}
}

void ADogFightPlayerController::HandleReturnToMainMenu()
{
	CleanupSessionOnReturnMain();
}

void ADogFightPlayerController::ClientPreStartGame_Implementation()
{
	// Let every client show the loading screen
	if (UDogFightGameInstance* GameInstance = Cast<UDogFightGameInstance>(GetGameInstance()))
	{
		GameInstance->ShowLoadingScreen();
	}
}

void ADogFightPlayerController::ClientHostUploadPlayerInfo_Implementation()
{
	if (GetNetMode() == NM_ListenServer || GetNetMode() == NM_Standalone)
	{
		GatherPlayerInfo();
	}
}

void ADogFightPlayerController::CleanupSessionOnReturnMain()
{
	const UWorld* World = GetWorld();
	UDogFightGameInstance* DogFightGameInstance = World != nullptr ? Cast<UDogFightGameInstance>(World->GetGameInstance()) : nullptr;
	if (ensure(DogFightGameInstance != nullptr))
	{
		// Let game instance handle the session stuff
		DogFightGameInstance->CleanupSessionOnReturnToMenu();
	}
}

void ADogFightPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Gather information and send to server
	GatherPlayerInfo();
}

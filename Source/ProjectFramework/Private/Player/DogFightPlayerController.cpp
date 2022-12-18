// Dog Fight Game Code By CYM.


#include "Player/DogFightPlayerController.h"

#include "GameInstance/DogFightGameInstance.h"
#include "Common/Localization.h"
#include "GameService/GameService.h"
#include "GameService/LanGameService.h"

void ADogFightPlayerController::ClientReturnToMainMenuWithReason_Implementation(EReturnToMainMenuReason::Type Reason)
{
	if (UGameInstance* const GameInstance = GetGameInstance())
	{
		// Remove the error handler first
		if (auto LanGameService = UGameService::GetGameService<ULanGameService>())
		{
			LanGameService->RemoveNetworkFailureHandler();
		}

		GameInstance->ReturnToMainMenu();

		// Add reason message
		if (auto LanGameService = UGameService::GetGameService<ULanGameService>())
		{
#if UE_EDITOR
			const FString ErrorContextString = UEnum::GetDisplayValueAsText(Reason).ToString();
#else
			const FString ErrorContextString = FString::Printf(TEXT("NetError_%s"),*UEnum::GetDisplayValueAsText(Reason).ToString());
#endif
			UE_LOG(LogProjectFramework, Log, TEXT("DisplayValueAsText: %s"), *ErrorContextString);
			
			LanGameService->AddPendingMessage(FText::FromStringTable(ST_UI_LOC, TEXT("NetErrorTitle")),
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
	if (auto LanGameService = UGameService::GetGameService<ULanGameService>())
	{
		// Let game instance handle the session stuff
		LanGameService->CleanupSessionOnReturnToMenu();
	}
}

void ADogFightPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Gather information and send to server
	GatherPlayerInfo();
}

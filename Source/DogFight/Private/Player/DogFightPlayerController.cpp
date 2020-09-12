// Dog Fight Game Code By CYM.


#include "DogFightPlayerController.h"

#include "DogFightGameInstance.h"

#define ST_UI_LOC		"/Game/DogFight/Localization/ST_UserInterface.ST_UserInterface"

void ADogFightPlayerController::RpcReturnToMainMenuWithReason_Implementation(EReturnToMainMenuReason::Type Reason)
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

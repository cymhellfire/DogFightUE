// Dog Fight Game Code By CYM.


#include "FunctionLibrary/FrameworkLibrary.h"

#include "GameInstance/DogFightGameInstance.h"

USaveGameManager* UFrameworkLibrary::GetSaveGameManager(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	UDogFightGameInstance* GameInstance = Cast<UDogFightGameInstance>(World->GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogProjectFramework, Error, TEXT("Invalid GameInstance class: %s"), *World->GetGameInstance()->StaticClass()->GetName());
		return nullptr;
	}

	return GameInstance->GetSaveGameManager();
}

FText UFrameworkLibrary::GetTextFromLocalizedString(const FLocalizedString& LocalizedString)
{
	return LocalizedString.GetLocalizeText();
}

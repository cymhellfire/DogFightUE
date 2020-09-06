// Dog Fight Game Code By CYM.


#include "FrameworkLibrary.h"

#include "DogFightGameInstance.h"

USaveGameManager* UFrameworkLibrary::GetSaveGameManager(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	UDogFightGameInstance* GameInstance = Cast<UDogFightGameInstance>(World->GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("Invalid GameInstance class: %s"), *World->GetGameInstance()->StaticClass()->GetName());
		return nullptr;
	}

	return GameInstance->GetSaveGameManager();
}

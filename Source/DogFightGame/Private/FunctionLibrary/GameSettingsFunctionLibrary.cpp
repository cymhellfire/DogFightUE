// Dog Fight Game Code By CYM.


#include "FunctionLibrary/GameSettingsFunctionLibrary.h"

#include "Common/DogFightGameLog.h"
#include "Common/DogFightGameUserSettings.h"

void UGameSettingsFunctionLibrary::SaveGameSettings(UObject* WorldContextObject)
{
	if (auto GameUserSettings = GEngine->GetGameUserSettings())
	{
		GameUserSettings->SaveSettings();
	}
}

void UGameSettingsFunctionLibrary::SetPlayerName(UObject* WorldContextObject, const FString& InName)
{
	if (InName.IsEmpty())
	{
		DFLogW(LogDogFightGame, TEXT("Empty player name is not allowed."))
		return;
	}

	if (auto GameUserSettings = Cast<UDogFightGameUserSettings>(GEngine->GetGameUserSettings()))
	{
		GameUserSettings->PlayerName = InName;
	}
}

FString UGameSettingsFunctionLibrary::GetPlayerName(UObject* WorldContextObject)
{
	if (auto GameUserSettings = Cast<UDogFightGameUserSettings>(GEngine->GetGameUserSettings()))
	{
		return GameUserSettings->PlayerName;
	}
	return "";
}

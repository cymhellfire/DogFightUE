// Dog Fight Game Code By CYM.


#include "FunctionLibrary/GameSettingsFunctionLibrary.h"

#include "Common/DogFightGameLog.h"
#include "Common/DogFightGameUserSettings.h"

#define GET_GAME_USER_SETTINGS		Cast<UDogFightGameUserSettings>(GEngine->GetGameUserSettings())

void UGameSettingsFunctionLibrary::SaveGameSettings()
{
	if (auto GameUserSettings = GEngine->GetGameUserSettings())
	{
		GameUserSettings->SaveSettings();
	}
}

void UGameSettingsFunctionLibrary::SetPlayerName(const FString& InName)
{
	if (InName.IsEmpty())
	{
		DFLogW(LogDogFightGame, TEXT("Empty player name is not allowed."))
		return;
	}

	if (auto GameUserSettings = GET_GAME_USER_SETTINGS)
	{
		GameUserSettings->PlayerName = InName;
	}
}

FString UGameSettingsFunctionLibrary::GetPlayerName()
{
	if (auto GameUserSettings = GET_GAME_USER_SETTINGS)
	{
		return GameUserSettings->PlayerName;
	}
	return "";
}

void UGameSettingsFunctionLibrary::SetLastAvatarId(const FGameLobbyPlayerAvatarId& InId)
{
	if (auto GameUserSettings = GET_GAME_USER_SETTINGS)
	{
		GameUserSettings->LastAvatarId = InId;
	}
}

FGameLobbyPlayerAvatarId UGameSettingsFunctionLibrary::GetLastAvatarId()
{
	if (auto GameUserSettings = GET_GAME_USER_SETTINGS)
	{
		return GameUserSettings->LastAvatarId;
	}
	return FGameLobbyPlayerAvatarId();
}

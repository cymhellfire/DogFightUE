// Dog Fight Game Code By CYM.


#include "Subsystem/DogFightSessionSubsystem.h"

#include "Common/LuaEventDef.h"
#include "GameService/LuaEventService.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystemUtils/Public/OnlineSubsystemUtils.h"

void UDogFightSessionSubsystem::UpdateSessionSettings(const FString& Key, const FString& Value)
{
	const IOnlineSessionPtr SessionInterface = GetCurrentSession();
	if (SessionInterface.IsValid() && HostSettings.IsValid())
	{
		const FName SessionName(NAME_GameSession);
		HostSettings->Set(FName(Key), Value, EOnlineDataAdvertisementType::ViaOnlineService);
		SessionInterface->UpdateSession(SessionName, *HostSettings, true);
	}
}

void UDogFightSessionSubsystem::HandlePostLoadMap(UWorld* World)
{
	Super::HandlePostLoadMap(World);

	// If loaded map has the same name of host settings
	if (HostSettings.IsValid())
	{
		FString LobbyMapName;
		if (PendingTravelURL.IsEmpty())
		{
			return;
		}

		int32 FirstQuestIndex = -1;
		PendingTravelURL.FindChar('?', FirstQuestIndex);
		if (FirstQuestIndex != -1)
		{
			LobbyMapName = PendingTravelURL.Mid(0, FirstQuestIndex);
		}

		// Fire a event when game lobby level is loaded
		FString LoadMapName = UWorld::RemovePIEPrefix(World->GetOutermost()->GetName());
		if (LoadMapName == LobbyMapName)
		{
			SEND_LUA_EVENT(ELuaEvent::LuaEvent_GameLobbyLevelLoaded)
		}
	}
	// Check for non-host players
	else
	{
		const auto SessionInterface = GetCurrentSession();
		if (SessionInterface.IsValid())
		{
			if (auto Settings = SessionInterface->GetSessionSettings(NAME_GameSession))
			{
				FString LobbyMapName;
				Settings->Get(SETTING_MAPNAME, LobbyMapName);
				FString LoadMapName = UWorld::RemovePIEPrefix(World->GetOutermost()->GetName());
				if (LobbyMapName == LoadMapName)
				{
					SEND_LUA_EVENT(ELuaEvent::LuaEvent_GameLobbyLevelLoaded)
				}
			}
		}
	}
}

IOnlineSessionPtr UDogFightSessionSubsystem::GetCurrentSession()
{
	IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld());
	check(OnlineSubsystem);

	const IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
	return SessionInterface;
}

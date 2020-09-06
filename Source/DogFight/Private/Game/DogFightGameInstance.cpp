// Dog Fight Game Code By CYM.


#include "DogFightGameInstance.h"

#include "SaveGameManager.h"

#define ST_UI_LOC		"/Game/DogFight/Localization/ST_UserInterface.ST_UserInterface"

UDogFightGameInstance::UDogFightGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create delegate functions
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UDogFightGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UDogFightGameInstance::OnStartSessionComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UDogFightGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UDogFightGameInstance::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UDogFightGameInstance::OnDestroySessionComplete);
}

bool UDogFightGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, FString InMapName, bool bIsLan, bool bIsPresence, int32 MaxNumPlayers)
{
	// Validate map name
	if (InMapName.IsEmpty())
	{
		UE_LOG(LogDogFight, Error, TEXT("Invalid map name to create a session."));
		return false;
	}

	// Get Session Interface
	const IOnlineSessionPtr Sessions = GetOnlineSessionPtr();

	if (Sessions && Sessions.IsValid() && UserId.IsValid())
	{
		// Create session settings
		SessionSettings = MakeShareable(new FOnlineSessionSettings());

		SessionSettings->bIsLANMatch = bIsLan;
		SessionSettings->bUsesPresence = bIsPresence;
		SessionSettings->NumPublicConnections = MaxNumPlayers;
		SessionSettings->NumPrivateConnections = 0;
		SessionSettings->bAllowInvites = true;
		SessionSettings->bAllowJoinInProgress = true;
		SessionSettings->bShouldAdvertise = true;
		SessionSettings->bAllowJoinViaPresence = true;
		SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

		SessionSettings->Set(SETTING_MAPNAME, InMapName, EOnlineDataAdvertisementType::ViaOnlineService);

		GameMapName = InMapName;

		// Set the delegate to the handle of the session interface
		OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(
			OnCreateSessionCompleteDelegate);

		return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
	}

	return false;
}

void UDogFightGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(
		TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	const IOnlineSessionPtr Sessions = GetOnlineSessionPtr();

	if (Sessions && Sessions.IsValid())
	{
		// Clear the delegate handler everytime it finished
		Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);

		if (bWasSuccessful)
		{
			// Set StartSession delegate handle
			OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(
				OnStartSessionCompleteDelegate);

			// Start session
			Sessions->StartSession(SessionName);
		}
	}
}

void UDogFightGameInstance::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));


	const IOnlineSessionPtr Sessions = GetOnlineSessionPtr();
	if (Sessions && Sessions.IsValid())
	{
		// Clear the delegate
		Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
	}

	// Open up a new map after the session is ready
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(*GameMapName), true, "listen");
	}
}

void UDogFightGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLan, bool bIsPresence)
{
	const IOnlineSessionPtr Sessions = GetOnlineSessionPtr();
	if (Sessions && Sessions.IsValid() && UserId.IsValid())
	{
		// Create Session Search
		SessionSearch = MakeShareable(new FOnlineSessionSearch());

		SessionSearch->bIsLanQuery = bIsLan;
		SessionSearch->MaxSearchResults = 20;
		SessionSearch->PingBucketSize = 50;

		if (bIsPresence)
		{
			SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
		}

		const TSharedRef<FOnlineSessionSearch> SearchSessionRef = SessionSearch.ToSharedRef();

		// Setup delegate for searching sessions
		OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(
			OnFindSessionsCompleteDelegate);

		Sessions->FindSessions(*UserId, SearchSessionRef);
	}
	else
	{
		// Directly invoke callback
		OnFindSessionsComplete(false);
	}
}

bool UDogFightGameInstance::JoinSpecifiedSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	bool bSuccessful = false;

	const IOnlineSessionPtr Sessions = GetOnlineSessionPtr();
	if (Sessions && Sessions.IsValid() && UserId.IsValid())
	{
		// Setup delegate handle
		OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(
			OnJoinSessionCompleteDelegate);

		// Join specified session
		bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
	}

	return bSuccessful;
}

void UDogFightGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UDogFightGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnFindSessionsComplete %d"), bWasSuccessful));

	const IOnlineSessionPtr Sessions = GetOnlineSessionPtr();
	if (Sessions && Sessions.IsValid())
	{
		Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Result: %d"), SessionSearch->SearchResults.Num()));

		if (SessionSearch->SearchResults.Num() > 0)
		{
			for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); ++SearchIdx)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
				                                 FString::Printf(
					                                 TEXT("Index: %d | SessionName: %s"), SearchIdx + 1,
					                                 *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName
					                                 )));
			}
		}
	}
}

void UDogFightGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<uint32>(Result)));

	const IOnlineSessionPtr Sessions = GetOnlineSessionPtr();
	if (Sessions && Sessions.IsValid())
	{
		// Clear the delegate handle
		Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

		// Get the first local player controller for later client travel
		APlayerController* const PlayerController = GetFirstLocalPlayerController();

		// Use a FString store URL for ClientTravel
		FString TravelURL;

		if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
		{
			// Call ClientTravel
			PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UDogFightGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	const IOnlineSessionPtr Sessions = GetOnlineSessionPtr();
	if (Sessions && Sessions.IsValid())
	{
		// Clear the delegate handle
		Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

		// If destroyed successful, open the menu level
		if (bWasSuccessful)
		{
			UGameplayStatics::OpenLevel(GetWorld(), FName(*MenuMapName), true);
		}
	}
}

IOnlineSessionPtr UDogFightGameInstance::GetOnlineSessionPtr() const
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		return OnlineSub->GetSessionInterface();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No Online Subsystem Available."));
	}

	return nullptr;
}

void UDogFightGameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	if (FailureType == ENetworkFailure::ConnectionLost)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Lost connection from host."));

		// Enqueue error message
		AddPendingMessage(FText::FromStringTable(ST_UI_LOC, FString("NetErrorTitle")),
			FText::FromStringTable(ST_UI_LOC, FString("NetError_LostConnect")));

		// Open the Main Menu level
		UGameplayStatics::OpenLevel(GetWorld(), FName(*MenuMapName), true);
	}
}

void UDogFightGameInstance::Init()
{
	// Create Save Manager
	SaveGameManager = NewObject<USaveGameManager>(this, FName(TEXT("SaveGameManager")));

	// Listen to Network Failure
	GEngine->OnNetworkFailure().AddUObject(this, &UDogFightGameInstance::HandleNetworkFailure);
}

void UDogFightGameInstance::HostOnlineGame(FString InMapName)
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	HostSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, InMapName, true, true, 4);
}


void UDogFightGameInstance::FindOnlineGames()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	FindSessions(Player->GetPreferredUniqueNetId().GetUniqueNetId(), true, true);
}

void UDogFightGameInstance::JoinOnlineGame()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	// Temporary select the first available session to join
	if (SessionSearch->SearchResults.Num() > 0)
	{
		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); ++i)
		{
			if (SessionSearch->SearchResults[i].Session.OwningUserId != Player->GetPreferredUniqueNetId())
			{
				const FOnlineSessionSearchResult SearchResult = SessionSearch->SearchResults[i];
				JoinSpecifiedSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, SearchResult);
				break;
			}
		}
	}
}

void UDogFightGameInstance::DestroySessionAndLeaveGame()
{
	const IOnlineSessionPtr Sessions = GetOnlineSessionPtr();
	if (Sessions && Sessions.IsValid())
	{
		OnDestroySessionCompleteDelegateHandle = Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

		Sessions->DestroySession(GameSessionName);
	}
}

void UDogFightGameInstance::AddPendingMessage(FText Title, FText Content)
{
	PendingMessages.Add(FMainMenuMessage{Title, Content});
}

void UDogFightGameInstance::RemovePendingMessageAt(int32 Index)
{
	if (PendingMessages.Num() <= Index)
	{
		return;
	}

	PendingMessages.RemoveAt(Index);
}

void UDogFightGameInstance::ClearPendingMessages()
{
	PendingMessages.Empty();
}

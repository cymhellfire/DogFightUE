#include "GameService/LanGameService.h"
#include "OnlineSessionSettings.h"
#include "Common/FrameworkType.h"
#include "Common/Localization.h"
#include "GameMode/DogFightGameModeBase.h"
#include "GameMode/DogFightGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"

namespace LanGameState
{
	const FName None = FName(TEXT("None"));
	const FName MainMenu = FName(TEXT("MainMenu"));
	const FName Playing = FName(TEXT("Playing"));
}

ULanGameService::ULanGameService()
{
	CurrentState = LanGameState::None;

	// Create delegate functions
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ULanGameService::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &ULanGameService::OnStartSessionComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &ULanGameService::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ULanGameService::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &ULanGameService::OnDestroySessionComplete);
	OnEndSessionCompleteDelegate = FOnEndSessionCompleteDelegate::CreateUObject(this, &ULanGameService::OnEndSessionComplete);
}

void ULanGameService::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Because this takes place outside the normal UWorld tick, we need to register what world we're ticking/modifying here to avoid issues in the editor
	FScopedConditionalWorldSwitcher WorldSwitcher(GetWorld());

	CheckChangeState();
}

bool ULanGameService::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, FString InMapName, bool bIsLan, bool bIsPresence, int32 MaxNumPlayers)
{
	// Validate map name
	if (InMapName.IsEmpty())
	{
		UE_LOG(LogProjectFramework, Error, TEXT("Invalid map name to create a session."));
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

void ULanGameService::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLan, bool bIsPresence)
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

bool ULanGameService::JoinSpecifiedSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
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

void ULanGameService::CleanupSessionOnReturnToMenu()
{
	const IOnlineSessionPtr Sessions = GetOnlineSessionPtr();
	if (Sessions && Sessions.IsValid())
	{
		const FString SessionName = LexToString(NAME_GameSession);
		EOnlineSessionState::Type SessionState = Sessions->GetSessionState(NAME_GameSession);
		UE_LOG(LogOnline, Log, TEXT("Session %s is '%s'"), *SessionName, EOnlineSessionState::ToString(SessionState));

		if (EOnlineSessionState::InProgress == SessionState)
		{
			UE_LOG(LogOnline, Log, TEXT("Ending session %s on return to main menu."), *SessionName);
			OnEndSessionCompleteDelegateHandle = Sessions->AddOnEndSessionCompleteDelegate_Handle(OnEndSessionCompleteDelegate);
			Sessions->EndSession(NAME_GameSession);
		}
		else if (EOnlineSessionState::Ending == SessionState)
		{
			UE_LOG(LogOnline, Log, TEXT("Waiting for session %s to end on return to main menu."), *SessionName);
			OnEndSessionCompleteDelegateHandle = Sessions->AddOnEndSessionCompleteDelegate_Handle(OnEndSessionCompleteDelegate);
		}
		else if (EOnlineSessionState::Ended == SessionState || EOnlineSessionState::Pending == SessionState)
		{
			UE_LOG(LogOnline, Log, TEXT("Destroying session %s on return to main menu."), *SessionName);
			OnDestroySessionCompleteDelegateHandle = Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
			Sessions->DestroySession(NAME_GameSession);

			RemoveNetworkFailureHandler();
		}
		else if (EOnlineSessionState::Starting == SessionState || EOnlineSessionState::Creating == SessionState)
		{
			UE_LOG(LogOnline, Log, TEXT("Waiting for session %s to start, and then we will end it to return to main menu."), *SessionName);
			OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
		}
	}
}

void ULanGameService::RemoveNetworkFailureHandler() const
{
	GEngine->OnNetworkFailure().Remove(NetworkFailureHandle);
}

void ULanGameService::HostOnlineGame(FString InMapName)
{
	if (UDogFightGameInstance* GameInstance = GetTypedOuter<UDogFightGameInstance>())
	{
		ULocalPlayer* const Player = GameInstance->GetFirstGamePlayer();

		HostSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), NAME_GameSession, InMapName, true, true, 4);
	}
}

void ULanGameService::FindOnlineGames()
{
	if (UDogFightGameInstance* GameInstance = GetTypedOuter<UDogFightGameInstance>())
	{
		ULocalPlayer* const Player = GameInstance->GetFirstGamePlayer();

		FindSessions(Player->GetPreferredUniqueNetId().GetUniqueNetId(), true, true);
	}
}

void ULanGameService::JoinOnlineGame()
{
	if (UDogFightGameInstance* GameInstance = GetTypedOuter<UDogFightGameInstance>())
	{
		ULocalPlayer* const Player = GameInstance->GetFirstGamePlayer();

		// Temporary select the first available session to join
		if (SessionSearch->SearchResults.Num() > 0)
		{
			for (int32 i = 0; i < SessionSearch->SearchResults.Num(); ++i)
			{
				if (SessionSearch->SearchResults[i].Session.OwningUserId != Player->GetPreferredUniqueNetId())
				{
					const FOnlineSessionSearchResult SearchResult = SessionSearch->SearchResults[i];
					JoinSpecifiedSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), NAME_GameSession, SearchResult);
					break;
				}
			}
		}
	}
}

void ULanGameService::DestroySessionAndLeaveGame()
{
	const IOnlineSessionPtr Sessions = GetOnlineSessionPtr();
	if (Sessions && Sessions.IsValid())
	{
		OnDestroySessionCompleteDelegateHandle = Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

		Sessions->DestroySession(NAME_GameSession);
	}
}

void ULanGameService::DirectlyConnectToHost(FString IpAddress)
{
	if (IpAddress.IsEmpty())
	{
		return;
	}

	// Check if this is a valid ip address
	TArray<FString> Parts;
	IpAddress.ParseIntoArray(Parts, TEXT("."));
	if (Parts.Num() != 4)
	{
		UE_LOG(LogProjectFramework, Error, TEXT("Invalid Ip address to connect: %s"), *IpAddress);
		return;
	}

	UDogFightGameInstance* GameInstance = GetTypedOuter<UDogFightGameInstance>();
	if (GameInstance == nullptr)
	{
		return;
	}
	APlayerController* PlayerController = GameInstance->GetFirstLocalPlayerController();
	if (PlayerController == nullptr)
	{
		UE_LOG(LogProjectFramework, Error, TEXT("No available local player controller."));
		return;
	}
	const FString Cmd = FString::Printf(TEXT("open %s"), *IpAddress);
	//GetWorld()->Exec(GetWorld(), *Cmd);
	PlayerController->ConsoleCommand(Cmd);

	// Setup network failure handler
	SetupNetworkFailureHandler();

	GoToState(LanGameState::Playing);
}

void ULanGameService::TravelToGameMap(FString InMapName, FString InGameMode)
{
	if (InMapName.IsEmpty())
	{
		return;
	}

	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode != nullptr)
	{
		if (ADogFightGameModeBase* DogFightGameModeBase = Cast<ADogFightGameModeBase>(GameMode))
		{
			DogFightGameModeBase->NotifyClientGameWillStart();
		}

		// Record player count
		if (auto GameInstance = GetTypedOuter<UDogFightGameInstance>())
		{
			GameInstance->GamePlayerCount = GameMode->GetNumPlayers();
		}
	}

	FString URL = InMapName;
	if (!InGameMode.IsEmpty())
	{
		URL += "?Game=" + InGameMode;
	}
	URL += "?listen";

	GetWorld()->ServerTravel(URL, true, false);
}

void ULanGameService::LeaveGameAndReturnToMainMenu()
{
	GoToState(LanGameState::MainMenu);
}

void ULanGameService::AddPendingMessage(FText Title, FText Content)
{
	PendingMessages.Add(FMainMenuMessage{Title, Content});
}

void ULanGameService::RemovePendingMessageAt(int32 Index)
{
	if (PendingMessages.Num() <= Index)
	{
		return;
	}

	PendingMessages.RemoveAt(Index);
}

void ULanGameService::ClearPendingMessages()
{
	PendingMessages.Empty();
}

void ULanGameService::GoToState(FName NewState)
{
	UE_LOG(LogOnline, Log, TEXT("GotoState: %s"), *NewState.ToString());

	PendingState = NewState;
}

void ULanGameService::SetupNetworkFailureHandler()
{
	// Listen to Network Failure
	NetworkFailureHandle = GEngine->OnNetworkFailure().AddUObject(this, &ULanGameService::HandleNetworkFailure);
}

void ULanGameService::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
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

void ULanGameService::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
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

		SetupNetworkFailureHandler();

		GoToState(LanGameState::Playing);
	}
}

void ULanGameService::OnFindSessionsComplete(bool bWasSuccessful)
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

void ULanGameService::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<uint32>(Result)));

	UDogFightGameInstance* GameInstance = GetTypedOuter<UDogFightGameInstance>();

	const IOnlineSessionPtr Sessions = GetOnlineSessionPtr();
	if (Sessions && Sessions.IsValid() && GameInstance)
	{
		// Clear the delegate handle
		Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

		// Get the first local player controller for later client travel
		APlayerController* const PlayerController = GameInstance->GetFirstLocalPlayerController();

		// Use a FString store URL for ClientTravel
		FString TravelURL;

		if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
		{
			SetupNetworkFailureHandler();

			GoToState(LanGameState::Playing);

			// Call ClientTravel
			PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
		}
	}
}

void ULanGameService::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
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
			//UGameplayStatics::OpenLevel(GetWorld(), FName(*MenuMapName), true);
		}
	}
}

void ULanGameService::OnEndSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogOnline, Log, TEXT("OnEndSessionComplete %s %d"), *SessionName.ToString(), bWasSuccessful);

	const IOnlineSessionPtr Sessions = GetOnlineSessionPtr();
	if (Sessions && Sessions.IsValid())
	{
		Sessions->ClearOnEndSessionCompleteDelegate_Handle(OnEndSessionCompleteDelegateHandle);
		Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
		Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
	}

	// Continue end session process
	CleanupSessionOnReturnToMenu();
}

IOnlineSessionPtr ULanGameService::GetOnlineSessionPtr() const
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

void ULanGameService::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	if (FailureType == ENetworkFailure::ConnectionLost)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Lost connection from host."));

#if UE_EDITOR
		const FString NetErrorString = UEnum::GetDisplayValueAsText(EReturnToMainMenuReason::LostConnect).ToString();
#else
		const FString NetErrorString = FString::Printf(TEXT("NetError_%s"), *UEnum::GetDisplayValueAsText(EReturnToMainMenuReason::LostConnect).ToString());
#endif

		UDogFightGameInstance* GameInstance = GetTypedOuter<UDogFightGameInstance>();
		if (!GameInstance)
		{
			return;
		}

		// Enqueue error message
		AddPendingMessage(FText::FromStringTable(ST_UI_LOC, FString("NetErrorTitle")),
			FText::FromStringTable(ST_UI_LOC, NetErrorString));

		// Open the Main Menu level
		UGameplayStatics::OpenLevel(GetWorld(), FName(*GameInstance->MenuMapName), true);
	}
}


void ULanGameService::CheckChangeState()
{
	if (PendingState != CurrentState && PendingState != LanGameState::None)
	{
		// End current state
		EndCurrentState();

		// Begin new state
		BeginNewState(PendingState);

		// Clear pending state
		PendingState = LanGameState::None;
	}
}

void ULanGameService::EndCurrentState()
{
	if (CurrentState == LanGameState::MainMenu)
	{
		EndMainMenuState();
	}
	else if (CurrentState == LanGameState::Playing)
	{
		EndPlayingState();
	}

	CurrentState = LanGameState::None;
}

void ULanGameService::BeginNewState(FName NewState)
{
	if (NewState == LanGameState::MainMenu)
	{
		BeginMainMenuState();
	}
	else if (NewState == LanGameState::Playing)
	{
		BeginPlayingState();
	}

	CurrentState = NewState;
}

void ULanGameService::BeginMainMenuState()
{
	if (auto GameInstance = GetTypedOuter<UDogFightGameInstance>())
	{
		GameInstance->LoadFrontEndMap(FString::Printf(TEXT("/Game/DogFight/Maps/%s"),*GameInstance->MenuMapName));
	}
}

void ULanGameService::EndMainMenuState()
{
	
}

void ULanGameService::BeginPlayingState()
{
	
}

void ULanGameService::EndPlayingState()
{
	UWorld* const World = GetWorld();
	ADogFightGameStateBase* const GameState = World != nullptr ? World->GetGameState<ADogFightGameStateBase>() : nullptr;

	if (GameState)
	{
		GameState->FinishGameAndReturnToMainMenu();
	}
	else
	{
		CleanupSessionOnReturnToMenu();
	}
}

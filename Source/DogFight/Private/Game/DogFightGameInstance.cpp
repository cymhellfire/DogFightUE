// Dog Fight Game Code By CYM.


#include "DogFightGameInstance.h"



#include "DogFightGameModeBase.h"
#include "DogFightGameStateBase.h"
#include "DogFightTypes.h"
#include "SaveGameManager.h"
#include "Blueprint/UserWidget.h"

#define ST_UI_LOC		"/Game/DogFight/Localization/ST_UserInterface.ST_UserInterface"

namespace DogFightGameInstanceState
{
	const FName None = FName(TEXT("None"));
	const FName MainMenu = FName(TEXT("MainMenu"));
	const FName Playing = FName(TEXT("Playing"));
}

UDogFightGameInstance::UDogFightGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create delegate functions
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UDogFightGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UDogFightGameInstance::OnStartSessionComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UDogFightGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UDogFightGameInstance::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UDogFightGameInstance::OnDestroySessionComplete);
	OnEndSessionCompleteDelegate = FOnEndSessionCompleteDelegate::CreateUObject(this, &UDogFightGameInstance::OnEndSessionComplete);

	CurrentState = DogFightGameInstanceState::None;
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

		SetupNetworkFailureHandler();

		GoToState(DogFightGameInstanceState::Playing);
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

	// Remove the ticker delegate
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
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
			SetupNetworkFailureHandler();

			GoToState(DogFightGameInstanceState::Playing);

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
			//UGameplayStatics::OpenLevel(GetWorld(), FName(*MenuMapName), true);
		}
	}
}

void UDogFightGameInstance::OnEndSessionComplete(FName SessionName, bool bWasSuccessful)
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

void UDogFightGameInstance::CleanupSessionOnReturnToMenu()
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

#if UE_EDITOR
		const FString NetErrorString = UEnum::GetDisplayValueAsText(EReturnToMainMenuReason::LostConnect).ToString();
#else
		const FString NetErrorString = FString::Printf(TEXT("NetError_%s"), *UEnum::GetDisplayValueAsText(EReturnToMainMenuReason::LostConnect).ToString());
#endif

		// Enqueue error message
		AddPendingMessage(FText::FromStringTable(ST_UI_LOC, FString("NetErrorTitle")),
			FText::FromStringTable(ST_UI_LOC, NetErrorString));

		// Open the Main Menu level
		UGameplayStatics::OpenLevel(GetWorld(), FName(*MenuMapName), true);
	}
}

void UDogFightGameInstance::Init()
{
	// Create Save Manager
	SaveGameManager = NewObject<USaveGameManager>(this, FName(TEXT("SaveGameManager")));

	// Register delegate for ticker callback
	TickDelegate = FTickerDelegate::CreateUObject(this, &UDogFightGameInstance::Tick);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);
}

bool UDogFightGameInstance::Tick(float DeltaSeconds)
{
	// Because this takes place outside the normal UWorld tick, we need to register what world we're ticking/modifying here to avoid issues in the editor
	FScopedConditionalWorldSwitcher WorldSwitcher(GetWorld());

	CheckChangeState();

	return true;
}

bool UDogFightGameInstance::LoadFrontEndMap(const FString& MapName)
{
	bool bSuccess = true;

	// Do nothing if the map is already loaded
	UWorld* const World = GetWorld();
	if (World)
	{
		FString const CurrentMapName = *World->PersistentLevel->GetOutermost()->GetName();
		if (CurrentMapName == MapName)
		{
			return bSuccess;
		}
	}

	FString Error;
	EBrowseReturnVal::Type BrowseRet = EBrowseReturnVal::Failure;
	const FURL URL(*FString::Printf(TEXT("%s"), *MapName));

	if (URL.Valid && !HasAnyFlags(RF_ClassDefaultObject))
	{
		BrowseRet = GetEngine()->Browse(*WorldContext, URL, Error);

		// Handle failure
		if (BrowseRet != EBrowseReturnVal::Success)
		{
			UE_LOG(LogLoad, Fatal, TEXT("%s"), *FString::Printf(TEXT("Failed to enter %s: %s. Please check the log for errors."), *MapName, *Error));
			bSuccess = false;
		}
	}

	return bSuccess;
}

void UDogFightGameInstance::SetupNetworkFailureHandler()
{
	// Listen to Network Failure
	NetworkFailureHandle = GEngine->OnNetworkFailure().AddUObject(this, &UDogFightGameInstance::HandleNetworkFailure);
}

void UDogFightGameInstance::RemoveNetworkFailureHandler() const
{
	GEngine->OnNetworkFailure().Remove(NetworkFailureHandle);
}

void UDogFightGameInstance::GoToState(FName NewState)
{
	UE_LOG(LogOnline, Log, TEXT("GotoState: %s"), *NewState.ToString());

	PendingState = NewState;
}

void UDogFightGameInstance::ShowLoadingScreen()
{
	// Create Loading screen widget
	const FStringClassReference WidgetClassRef(TEXT("/Game/DogFight/UI/TransitionMap/BP_TransitionMapWidget.BP_TransitionMapWidget_C"));
	if (UClass* WidgetClass = WidgetClassRef.TryLoadClass<UUserWidget>())
	{
		UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetFirstLocalPlayerController(), WidgetClass);
		NewWidget->AddToViewport();
	}
}

void UDogFightGameInstance::CheckChangeState()
{
	if (PendingState != CurrentState && PendingState != DogFightGameInstanceState::None)
	{
		// End current state
		EndCurrentState();

		// Begin new state
		BeginNewState(PendingState);

		// Clear pending state
		PendingState = DogFightGameInstanceState::None;
	}
}

void UDogFightGameInstance::EndCurrentState()
{
	if (CurrentState == DogFightGameInstanceState::MainMenu)
	{
		EndMainMenuState();
	}
	else if (CurrentState == DogFightGameInstanceState::Playing)
	{
		EndPlayingState();
	}

	CurrentState = DogFightGameInstanceState::None;
}

void UDogFightGameInstance::BeginNewState(FName NewState)
{
	if (NewState == DogFightGameInstanceState::MainMenu)
	{
		BeginMainMenuState();
	}
	else if (NewState == DogFightGameInstanceState::Playing)
	{
		BeginPlayingState();
	}

	CurrentState = NewState;
}

void UDogFightGameInstance::BeginMainMenuState()
{
	LoadFrontEndMap(FString::Printf(TEXT("/Game/DogFight/Maps/%s"),*MenuMapName));
}

void UDogFightGameInstance::EndMainMenuState()
{
	
}

void UDogFightGameInstance::BeginPlayingState()
{
	
}

void UDogFightGameInstance::EndPlayingState()
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

void UDogFightGameInstance::DirectlyConnectToHost(FString IpAddress)
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
		UE_LOG(LogDogFight, Error, TEXT("Invalid Ip address to connect: %s"), *IpAddress);
		return;
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController == nullptr)
	{
		UE_LOG(LogDogFight, Error, TEXT("No available local player controller."));
		return;
	}
	const FString Cmd = FString::Printf(TEXT("open %s"), *IpAddress);
	//GetWorld()->Exec(GetWorld(), *Cmd);
	PlayerController->ConsoleCommand(Cmd);

	// Setup network failure handler
	SetupNetworkFailureHandler();
	
	GoToState(DogFightGameInstanceState::Playing);
}

void UDogFightGameInstance::TravelToGameMap(FString InMapName, FString InGameMode)
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
	}

	FString URL = InMapName;
	if (!InGameMode.IsEmpty())
	{
		URL += "?Game=" + InGameMode;
	}
	URL += "?listen";

	GetWorld()->ServerTravel(URL, true, false);
}

void UDogFightGameInstance::LeaveGameAndReturnToMainMenu()
{
	GoToState(DogFightGameInstanceState::MainMenu);
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

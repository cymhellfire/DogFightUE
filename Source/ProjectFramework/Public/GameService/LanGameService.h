#pragma once

#include "GameService.h"
#include "LanGameService.generated.h"

namespace LanGameState
{
	extern const FName None;
	extern const FName MainMenu;
	extern const FName Playing;
}

UCLASS()
class PROJECTFRAMEWORK_API ULanGameService : public UGameService
{
	GENERATED_BODY()
public:
	ULanGameService();

	virtual void Tick(float DeltaTime) override;

	/**
	 * Function to host a game.
	 *
	 * @param UserId		ID of User who started the request
	 * @param SessionName	Name of the session
	 * @param bIsLan		Is this a Lan game?
	 * @param bIsPresence	Is the session created with presence enabled?
	 * @param MaxNumPlayers	Capacity of the session. 
	 */
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, FString InMapName, bool bIsLan, bool bIsPresence, int32 MaxNumPlayers);

	/**
	 * Find online sessions.
	 *
	 * @param UserId		ID of User who started this request
	 * @param bIsLan		Searching for Lan games?
	 * @param bIsPresence	Searching for games with presence enabled?
	 */
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLan, bool bIsPresence);

	/**
	 * Joins a session via a search result.
	 *
	 * @param UserId		ID of User who started this request
	 * @param SessionName	Name of session to join
	 * @param SearchResult	Session to join
	 */
	bool JoinSpecifiedSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	/**
	 * Shutdown current session and free any using net driver.
	 */
	void CleanupSessionOnReturnToMenu();

	/** Remove the handler for network failure. */
	void RemoveNetworkFailureHandler() const;

	UFUNCTION(BlueprintCallable, Category="Network|Test")
	void HostOnlineGame(FString InMapName);

	UFUNCTION(BlueprintCallable, Category="Network|Test")
	void FindOnlineGames();

	UFUNCTION(BlueprintCallable, Category="Network|Test")
	void JoinOnlineGame();

	UFUNCTION(BlueprintCallable, Category="Network|Test")
	void DestroySessionAndLeaveGame();

	UFUNCTION(BlueprintCallable, Category="DogFight|Network")
	void DirectlyConnectToHost(FString IpAddress);

	UFUNCTION(BlueprintCallable, Category="DogFight|Game")
	void TravelToGameMap(FString InMapName, FString InGameMode);

	UFUNCTION(BlueprintCallable, Category="DogFight|Game")
	void LeaveGameAndReturnToMainMenu();

	UFUNCTION(BlueprintCallable, Category="DogFight|Game")
	void GoToState(FName NewState);

	UFUNCTION(BlueprintCallable, Category="DogFight|Game", meta=(DisplayName="Add Main Menu Message"))
	void AddPendingMessage(FText Title, FText Content);

	UFUNCTION(BlueprintCallable, Category="DogFight|Game", meta=(DisplayName="Remove Main Menu Message At Index"))
	void RemovePendingMessageAt(int32 Index);

	UFUNCTION(BlueprintCallable, Category="DogFight|Game", meta=(DisplayName="Get All Main Menu Messages"))
	TArray<FMainMenuMessage> GetAllPendingMessages() { return PendingMessages; }

	UFUNCTION(BlueprintCallable, Category="DogFight|Game", meta=(DisplayName="Clear All Main Menu Messages"))
	void ClearPendingMessages();

protected:
	/** Register the handler to network failure. */
	void SetupNetworkFailureHandler();

	/* Callback function for CreateSessionComplete delegate */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/* Callback function for StartSessionComplete delegate */
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

	/* Callback function for FindSessionsComplete delegate */
	void OnFindSessionsComplete(bool bWasSuccessful);

	/* Callback function for JoinSessionComplete delegate */
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/* Callback function for DestroySessionComplete delegate */
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	/* Callback function for EndSessionComplete delegate */
	void OnEndSessionComplete(FName SessionName, bool bWasSuccessful);

	/* Wrapper for getting IOnlineSessionPtr from current Online Subsystem. */
	IOnlineSessionPtr GetOnlineSessionPtr() const;

	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	void CheckChangeState();

	void EndCurrentState();
	void BeginNewState(FName NewState);

	void BeginMainMenuState();
	void EndMainMenuState();
	void BeginPlayingState();
	void EndPlayingState();

protected:
	/* Delegate triggered when session created. */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/* Delegate triggered when session started. */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	/* Delegate triggered when searching sessions finished. */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	/* Delegate triggered when joined session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	/* Delegate triggered when session destroyed. */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	/* Delegate triggered when session ended. */
	FOnEndSessionCompleteDelegate OnEndSessionCompleteDelegate;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	FDelegateHandle OnEndSessionCompleteDelegateHandle;

	FDelegateHandle NetworkFailureHandle;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	/* Name of map to use in new session. */
	FString GameMapName;

	FName PendingState;
	FName CurrentState;

	/** Messages will pop up when return to main menu. */
	UPROPERTY()
	TArray<FMainMenuMessage> PendingMessages;
};

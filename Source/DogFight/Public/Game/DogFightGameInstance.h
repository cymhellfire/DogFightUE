// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"
#include "Engine/GameInstance.h"
#include "DogFightGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UDogFightGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UDogFightGameInstance(const FObjectInitializer& ObjectInitializer);

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

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

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

	/* Wrapper for getting IOnlineSessionPtr from current Online Subsystem. */
	IOnlineSessionPtr GetOnlineSessionPtr() const;
private:
	/* Name of menu map. */
	FString MenuMapName;

	/* Name of map to use in new session. */
	FString GameMapName;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

#pragma region Blueprint
public:
	UFUNCTION(BlueprintCallable, Category="GameInstance")
	void SetMenuMapName(FString InMapName);

	UFUNCTION(BlueprintCallable, Category="Network|Test")
	void HostOnlineGame(FString InMapName);

	UFUNCTION(BlueprintCallable, Category="Network|Test")
	void FindOnlineGames();

	UFUNCTION(BlueprintCallable, Category="Network|Test")
	void JoinOnlineGame();

	UFUNCTION(BlueprintCallable, Category="Network|Test")
	void DestroySessionAndLeaveGame();
#pragma endregion Blueprint
};
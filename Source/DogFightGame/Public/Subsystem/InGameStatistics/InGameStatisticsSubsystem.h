// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameLobbyPlayerInfo.h"
#include "Subsystem/LuaGameInstanceSubsystem.h"
#include "InGameStatisticsSubsystem.generated.h"

class AGameLobbyPlayerState;

USTRUCT(BlueprintType)
struct FInGameStatisticsPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameLobbyPlayerInfo GameLobbyPlayerInfo;
};

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UInGameStatisticsSubsystem : public ULuaGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category="InGameStatisticsSubsystem")
	void RegisterPlayer(APlayerState* InPlayerState);

	UFUNCTION(BlueprintCallable, Category="InGameStatisticsSubsystem")
	void UnregisterPlayer(APlayerState* InPlayerState);

	UFUNCTION(BlueprintCallable, Category="InGameStatisticsSubsystem")
	void ClearAllStatistics();

	FInGameStatisticsPlayerInfo const* GetStatisticsByPlayerState(const APlayerState* InPlayerState) const;

private:
	FInGameStatisticsPlayerInfo* GetMutableStatisticsByPlayerState(const APlayerState* InPlayerState);

	void OnPlayerLogin(AGameModeBase* GameMode, APlayerController* PC);

	void RegisterGameLobbyPlayerState(AGameLobbyPlayerState* InPlayerState);

	void UnregisterGameLobbyPlayerState(AGameLobbyPlayerState* InPlayerState);

	UFUNCTION()
	void OnPlayerStateEndPlay(AActor* InActor, EEndPlayReason::Type InReason);

	void OnPlayerLobbyInfoChanged(AGameLobbyPlayerState* InPlayerState, const FGameLobbyPlayerInfo& InInfo);

private:

	TMap<FString, FInGameStatisticsPlayerInfo> PlayerStatisticsMap;
};

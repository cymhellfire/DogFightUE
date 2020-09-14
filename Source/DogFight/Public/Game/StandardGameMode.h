// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "DogFightGameModeBase.h"
#include "StandardGameMode.generated.h"

class AStandardModePlayerController;

namespace GamePhase
{
	extern DOGFIGHT_API const FName EnteringMap;			// Players are entering this map, actors are not yet ticking
	extern DOGFIGHT_API const FName WaitingForStart;		// Actors are ticking, but the match has not yet begun
	extern DOGFIGHT_API const FName InProgress;			// Normal gameplay is occurring.
	extern DOGFIGHT_API const FName WaitingPostMatch;	// Match has ended but actors are still ticking
}

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStandardGameMode : public ADogFightGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StandardMode")
	int32 GameStartDelay;

public:
	AStandardGameMode(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="Standard Game Mode")
	void EnablePlayerClickMovement();

	UFUNCTION(BlueprintCallable, Category="Standard Game Mode")
	void DisablePlayerClickMovement();

	/** Override version. Remove corresponding record in PlayerControllerList. */
	virtual void Logout(AController* Exiting) override;

	/** Override version. Send information to all connected clients. */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void PreInitializeComponents() override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	FName GetGamePhase() const { return CurrentGamePhase; }

	virtual void StartGame();
protected:
	virtual void BeginPlay() override;
	
	/** All PlayerController instances in current game. */
	TArray<AStandardModePlayerController*> StandardPlayerControllerList;

	FName CurrentGamePhase;

	FTimerHandle DefaultTimerHandle;

	/** Update remaining time. */
	virtual void DefaultTimer();

	virtual void SetGamePhase(FName NewPhase);

	virtual void OnGamePhaseChanged();

	virtual void HandlePhaseWaitingForStart();

	virtual void HandlePhaseInProgress();

	virtual void HandlePhaseWaitingPostMatch();

#pragma region Debug
public:
	UFUNCTION(Exec)
	void SetAllPlayerClickMove(bool bEnable);
#pragma endregion Debug
};

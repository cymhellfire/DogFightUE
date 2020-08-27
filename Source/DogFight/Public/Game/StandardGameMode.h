// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StandardGameMode.generated.h"

class AStandardModePlayerController;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStandardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Standard Game Mode")
	void EnablePlayerClickMovement();

	UFUNCTION(BlueprintCallable, Category="Standard Game Mode")
	void DisablePlayerClickMovement();

	/** Override version. Spawn and record PlayerController. */
	virtual APlayerController* SpawnPlayerController(ENetRole InRemoteRole, const FString& Options) override;

	/** Override version. Remove corresponding record in PlayerControllerList. */
	virtual void Logout(AController* Exiting) override;
protected:
	/** All PlayerController instances in current game. */
	TArray<AStandardModePlayerController*> PlayerControllerList;

#pragma region Debug
public:
	UFUNCTION(Exec)
	void SetAllPlayerClickMove(bool bEnable);
#pragma endregion Debug
};

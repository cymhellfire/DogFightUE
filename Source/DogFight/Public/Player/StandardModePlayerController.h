// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "DogFightPlayerController.h"
#include "StandardModePlayerController.generated.h"

class AStandardModePlayerCharacter;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStandardModePlayerController : public ADogFightPlayerController
{
	GENERATED_UCLASS_BODY()

	void SetClickMovementEnabled(bool bEnabled);

	UPROPERTY(Category=PlayerController, EditAnywhere, BlueprintReadOnly, Replicated)
	TSubclassOf<AStandardModePlayerCharacter> CharacterPawnClass;

	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	void CloseInGameMenu();

	virtual void InitPlayerState() override;

	virtual void OnRep_PlayerState() override;

	/** Current game enter InProgress phase. */
	virtual void GameStart();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void ProcessPlayerInput(const float DeltaTime, const bool bGamePaused) override;

	virtual void GatherPlayerInfo() override;

	/** Callback function triggers when player name has been changed. */
	UFUNCTION()
	virtual void OnPlayerNameChanged(const FString& NewName);

	void OnSetDestinationPressed();
	void OnOpenInGameMenuPressed();

	UFUNCTION(Server, Reliable)
	void CmdMoveToMouseCursor(FVector Destination);

	UFUNCTION(Server, Reliable)
	void CmdSpawnCharacterPawn();

	UFUNCTION(Server, Reliable)
	void CmdSetCharacterName(const FString& NewName);

	UFUNCTION()
	void OnRep_CharacterPawn();

private:
	UPROPERTY(Category=PlayerController, VisibleAnywhere, ReplicatedUsing=OnRep_CharacterPawn)
	AStandardModePlayerCharacter* CharacterPawn;

	/** Is click movement enabled for this player controller? */
	UPROPERTY(Category=PlayerController, VisibleAnywhere, Replicated)
	bool bClickMoveEnabled;

	/** Is the in game menu displayed? */
	bool bInGameMenuShown;

	/** If player name is set before controlling pawn spawn, this variable will be the temporary buffer. */
	FString PendingUnitName;

	UPROPERTY()
	UUserWidget* InGameMenuWidget;

#pragma region DebugCommand
public:

	UFUNCTION(Exec)
	void ExecSetCharacterName(FString NewName);
#pragma endregion DebugCommand
};

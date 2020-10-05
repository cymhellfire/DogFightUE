// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "DogFightPlayerController.h"
#include "DogFightTypes.h"
#include "UIType.h"
#include "GameCardUserPlayerControllerInterface.h"
#include "GameTargetProviderInterface.h"

#include "StandardModePlayerController.generated.h"

class AStandardModePlayerCharacter;

UENUM(BlueprintType)
enum class EStandardModePlayerControllerInputMode : uint8
{
	IM_Disable				UMETA(DisplayName="Disable"),
	IM_ClickMove			UMETA(DisplayName="ClickMove"),
	IM_TargetingActor		UMETA(DisplayName="TargetingActor"),
	IM_TargetingPosition	UMETA(DisplayName="TargetingPosition"),
	IM_TargetingDirection	UMETA(DisplayName="TargetingDirection"),
};

/**
 * Player Controller for Standard Mode
 */
UCLASS()
class DOGFIGHT_API AStandardModePlayerController : public ADogFightPlayerController, public IGameTargetProviderInterface, public IGameCardUserPlayerControllerInterface
{
	GENERATED_UCLASS_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerDeadSignature, int32, PlayerId);
	FPlayerDeadSignature OnPlayerDead;

	UFUNCTION(Client, Reliable)
	void RpcSetClickMovementEnabled(bool bEnabled);

	UPROPERTY(Category=PlayerController, EditAnywhere, BlueprintReadOnly, Replicated)
	TSubclassOf<AStandardModePlayerCharacter> CharacterPawnClass;

	UFUNCTION(BlueprintCallable, Category="DogFight|UI")
	void CloseInGameMenu();

	virtual void InitPlayerState() override;

	virtual void OnRep_PlayerState() override;

	/** Current game enter InProgress phase. */
	virtual void GameStart();

	UFUNCTION(BlueprintCallable, Category="DogFight|PlayerController")
	AStandardModePlayerCharacter* GetCharacterPawn() const { return CharacterPawn; }

	/** Send a message to all other players in current game. */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category="DogFight|PlayerController")
	void CmdBroadcastGameMessageToAll(const FString& GameMessage, const TArray<FString>& Arguments);

	UFUNCTION(Client, Reliable)
	void RpcReceivedGameMessage(FGameMessage Message);

	UFUNCTION(Client, Reliable)
	void RpcSetupTimelineDisplay();

	UFUNCTION(Client, Reliable)
	void RpcShowCardDisplayWidgetWithSelectMode(ECardSelectionMode SelectionMode);

	UFUNCTION(Client, Reliable)
	void RpcHideCardDisplayWidget();

	UFUNCTION(Client, Reliable)
	void RpcSetCardDisplayWidgetSelectable(bool bSelectable);

	UFUNCTION(Server, Reliable)
	void CmdUploadSelectedCardIndex(const TArray<int32>& SelectedIndexList);

	/** Stop the character movement immediately. */
	void StopCharacterMovementImmediately();

#pragma region Interfaces
	virtual FCardTargetInfoAcquiredSignature& GetTargetInfoAcquiredDelegate() override { return OnCardTargetInfoAcquired; }

	virtual void RequestActorTarget() override;
	virtual void RequestPositionTarget() override;
	virtual void RequestDirectionTarget() override;

	virtual APawn* GetActualPawn() override;
#pragma endregion Interfaces
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void ProcessPlayerInput(const float DeltaTime, const bool bGamePaused) override;

	virtual void GatherPlayerInfo() override;

	/** Set InputMode to IM_Disable and clear operation hint message. */
	void DisableInputMode();

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

	UFUNCTION(Server, Reliable)
	void CmdSetCharacterHealth(int32 NewHealth);

	/** Tell server that local player is ready for game. */
	UFUNCTION(Server, Reliable)
	void CmdReadyForGame(const FString& PlayerName);

	UFUNCTION(Server, Reliable)
	void CmdRegisterToGameTimeline();

	/** Forcibly finish current round of this player. */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="DogFight|Game")
	void CmdFinishMyRound();

	UFUNCTION()
	void OnRep_CharacterPawn();

	UFUNCTION()
	void OnCardInfoListChanged();

	UFUNCTION()
	void OnCardSelectionConfirmed(const TArray<int32>& SelectedIndexList);

#pragma region Target Acquire
	UFUNCTION(Client, Reliable)
	void RpcRequestActorTarget();

	UFUNCTION(Server, Reliable)
	void CmdUploadActorTarget(AActor* TargetActor);

	UFUNCTION(Client, Reliable)
	void RpcRequestPositionTarget();

	UFUNCTION(Server, Reliable)
	void CmdUploadPositionTarget(FVector TargetPosition);

	UFUNCTION(Client, Reliable)
	void RpcRequestDirectionTarget();

	UFUNCTION(Server, Reliable)
	void CmdUploadDirectionTarget(FVector TargetDirection);
#pragma endregion Target Acquire

	UFUNCTION()
	void OnCharacterDead();

private:
	FCardTargetInfoAcquiredSignature OnCardTargetInfoAcquired;

	UPROPERTY(Category=PlayerController, VisibleAnywhere, ReplicatedUsing=OnRep_CharacterPawn)
	AStandardModePlayerCharacter* CharacterPawn;

	UPROPERTY(Category=PlayerController, VisibleAnywhere)
	EStandardModePlayerControllerInputMode InputMode;

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

	UFUNCTION(Exec)
	void ExecSetCurrentHealth(int32 NewHealth);

	UFUNCTION(Exec)
	void ExecRequireActorTarget();

	UFUNCTION(Exec)
	void ExecRequirePositionTarget();

	UFUNCTION(Exec)
	void ExecRequireDirectionTarget();

	UFUNCTION(Exec)
	void ExecToggleCardDisplayWidget(bool bVisible);
#pragma endregion DebugCommand
};

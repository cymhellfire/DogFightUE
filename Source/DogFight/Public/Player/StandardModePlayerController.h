// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "DogFightPlayerController.h"
#include "DogFightTypes.h"
#include "UI/UIType.h"
#include "Actors/Interfaces/GameCardUserPlayerControllerInterface.h"
#include "Actors/Interfaces/GameTargetProviderInterface.h"

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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerHealthChangedSignature, int32, PlayerId, int32, NewHealth);
	FPlayerHealthChangedSignature OnPlayerHealthChanged;

	UFUNCTION(Client, Reliable)
	void ClientSetClickMovementEnabled(bool bEnabled);

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
	void ServerBroadcastGameMessageToAll(const FString& GameMessage, const TArray<FText>& Arguments);

	UFUNCTION(Client, Reliable)
	void ClientReceivedGameMessage(FGameMessage Message);

	UFUNCTION(Client, Reliable)
	void ClientReceivedGameTitleMessage(FGameTitleMessage Message);

	UFUNCTION(Client, Reliable)
	void ClientSetupTimelineDisplay();

	UFUNCTION(Client, Reliable)
	void ClientShowCardDisplayWidgetWithSelectMode(ECardSelectionMode SelectionMode);

	UFUNCTION(Client, Reliable)
	void ClientHideCardDisplayWidget();

	UFUNCTION(Client, Reliable)
	void ClientSetCardDisplayWidgetSelectable(bool bSelectable);

	UFUNCTION(Server, Reliable)
	void ServerUploadSelectedCardIndex(const TArray<int32>& SelectedIndexList);

	UFUNCTION(Client, Reliable)
	void ClientStartDiscardCards(int32 CountToDiscard);

	UFUNCTION(Client, Reliable)
	void ClientStopDiscardCards();

	/** Stop the character movement immediately. */
	void StopCharacterMovementImmediately();

#pragma region Interfaces
	virtual FCardTargetInfoAcquiredSignature& GetTargetInfoAcquiredDelegate() override { return OnCardTargetInfoAcquired; }
	virtual FOnTargetActorSelectedSignature& GetOnTargetActorSelectedDelegate() override { return OnTargetActorSelected; }

	virtual void RequestActorTarget() override;
	virtual void RequestPositionTarget() override;
	virtual void RequestDirectionTarget() override;

	virtual FCardInstructionTargetInfo RequestRandomActorTarget(bool bIgnoreSelf) override;
	virtual FCardInstructionTargetInfo RequestRandomPositionTarget() override;
	virtual FCardInstructionTargetInfo RequestRandomDirectionTarget() override;

	virtual APawn* GetActualPawn() override;
	virtual void BroadcastCardTargetingResult(FText CardName, FText TargetText, ECardInstructionTargetType TargetType) override;
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
	void ServerMoveToMouseCursor(FVector Destination);

	UFUNCTION(Server, Reliable)
	void ServerSpawnCharacterPawn();

	UFUNCTION(Server, Reliable)
	void ServerSetCharacterName(const FString& NewName);

	UFUNCTION(Server, Reliable)
	void ServerSetCharacterHealth(int32 NewHealth);

	/** Tell server that local player is ready for game. */
	UFUNCTION(Server, Reliable)
	void ServerReadyForGame(const FString& PlayerName);

	UFUNCTION(Server, Reliable)
	void ServerRegisterToGameTimeline();

	/** Forcibly finish current round of this player. */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="DogFight|Game")
	void ServerFinishMyRound();

	UFUNCTION()
	void OnRep_CharacterPawn();

	UFUNCTION()
	void OnCardInfoListChanged();

	UFUNCTION()
	void OnCardSelectionConfirmed(TArray<int32>& SelectedIndexList);

#pragma region Target Acquire
	UFUNCTION(Client, Reliable)
	void ClientSelectActorTarget();

	UFUNCTION(Server, Reliable)
	void ServerSyncActorTarget(AActor* TargetActor);

	UFUNCTION(Client, Reliable)
	void ClientSelectPositionTarget();

	UFUNCTION(Server, Reliable)
	void ServerSyncPositionTarget(FVector TargetPosition);

	UFUNCTION(Client, Reliable)
	void ClientSelectDirectionTarget();

	UFUNCTION(Server, Reliable)
	void ServerSyncDirectionTarget(FVector TargetDirection);

	APawn* GetRandomPlayerPawn(bool bIgnoreSelf);
#pragma endregion Target Acquire

	UFUNCTION()
	void OnCharacterDead();

	UFUNCTION()
	void OnHealthChanged(int32 NewHealth);

private:
	FCardTargetInfoAcquiredSignature OnCardTargetInfoAcquired;

	/** Triggered after this player selected actor and before send this actor to card. */
	FOnTargetActorSelectedSignature OnTargetActorSelected;

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

	UFUNCTION(Exec)
	void ExecSetRagdoll(bool bActive);
#pragma endregion DebugCommand
};

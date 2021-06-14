// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "Card/GameCardTypes.h"
#include "DogFightTypes.h"
#include "UIType.h"
#include "Ability/AbilityDisplayInfo.h"
#include "GameFramework/HUD.h"
#include "StandardHUD.generated.h"

class UGamePhaseMessageWidget;
class UOperationHintMessageWidget;
class UGameMessageWindowWidget;
class UGameRoundsTimelineWidget;
class UCardDisplayWidget;
class UGameTitleMessageWidget;
class AStandardGameState;
class UInGameHudWidget;
class UAbilityPanelWidget;
class UAbilitySelectWindowWidget;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStandardHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

public:

	/** Display specified hint message on screen. */
	void DisplayHintMessage(FName HintMessage, TArray<FText> MessageArguments = {});

	void HideOperationHintMessageWidget();

	void ShowGameMessage(FGameMessage Message);

	void InitializeTimelineDisplayTimeline();

	void SetCardDisplayInfoList(const TArray<FCardInstanceDisplayInfo>& CardInfoList);

	void RefreshCardUsingAbilityInfo();

	void RefreshCardUsableState();

	void ToggleCardDisplayWidgetVisibility(bool bVisible);

	void SetCardSelectionMode(ECardSelectionMode SelectionMode);

	/** Set the card item selectable. */
	void SetCardDisplayWidgetSelectable(bool bSelectable);

	FORCEINLINE UCardDisplayWidget* GetCardDisplayWidget() const { return CardDisplayWidget; }

	/** Switch the visibility of TimelineDisplayWidget. */
	void SetTimelineVisibility(bool bVisible);

	void ShowGameTitleMessage(FGameTitleMessage Message);

	void StartDiscardCards(int32 CountToDiscard);

	void StopDiscardCards();

	void StartRequestResponseCard(int32 MaxCardCount);

	void AddNewPlayerAbility(FAbilityDisplayInfo AbilityInfo, int32 AbilitySlot);

	void RemovePlayerAbility(int32 AbilitySlot);

	void UpdateAbilityCooldown(int32 AbilitySlot, int32 CurrentCooldown);

	void UpdateAbilityAvailability(int32 AbilitySlot, bool NewAvailability);

	UAbilityPanelWidget* GetAbilityPanelWidget() const { return AbilityPanelWidget; }

	void ShowAbilitySelectWindow(TArray<FAbilityDisplayInfo> AbilityDisplayInfos);

	void HideAbilitySelectWindow();

	UAbilitySelectWindowWidget* GetAbilitySelectWindowWidget() const { return AbilitySelectWindowWidget; }

protected:
	virtual void BeginPlay() override;

	bool ShouldPhaseMessageDisplay(FName GamePhase) const;

public:
	virtual void DrawHUD() override;

	/* Margin of Mini Map texture. */
	UPROPERTY(Category=Layout, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "1024"))
	float MiniMapMargin;

	/* Current UI scale. */
	float UIScale;

	/* Mini Map Frustum Points */
	UPROPERTY(Category=Debug, VisibleAnywhere, BlueprintReadOnly)
	TArray<FVector2D> MiniMapPoints;

	UPROPERTY(Category=Debug, EditAnywhere, BlueprintReadWrite)
	bool DrawDebugLines;

	UPROPERTY(Category="UI", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UInGameHudWidget> InGameHudWidgetClass;

	/** The GamePhaseMessageWidget class to use in game. */
	UPROPERTY(Category="UI", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGamePhaseMessageWidget> GamePhaseMessageWidgetClass;

	/** The OperationHintMessageWidget class to use in game. */
	UPROPERTY(Category="UI", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UOperationHintMessageWidget> OperationHintMessageWidgetClass;

	UPROPERTY(Category="UI", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameMessageWindowWidget> GameMessageWindowWidgetClass;

	UPROPERTY(Category="UI", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameRoundsTimelineWidget> GameRoundsTimelineWidgetClass;

	UPROPERTY(Category="UI", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCardDisplayWidget> CardDisplayWidgetClass;

	UPROPERTY(Category="UI", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameTitleMessageWidget> GameTitleMessageWidgetClass;

	UPROPERTY(Category="UI", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAbilityPanelWidget> AbilityPanelWidgetClass;

	UPROPERTY(Category="UI", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAbilitySelectWindowWidget> AbilitySelectWindowWidgetClass;

protected:
	void DrawMiniMap();

	UFUNCTION()
	void OnGamePhaseChanged(const FName& NewGamePhase);

	UFUNCTION()
	void OnCountdownContentStringChanged();

	UPROPERTY()
	UInGameHudWidget* InGameHudWidget;

	UPROPERTY()
	UGamePhaseMessageWidget* GamePhaseMessageWidget;
	UPROPERTY()
	UOperationHintMessageWidget* OperationHintMessageWidget;
	UPROPERTY()
	UGameMessageWindowWidget* GameMessageWindowWidget;
	UPROPERTY()
	UGameRoundsTimelineWidget* GameRoundsTimelineWidget;
	UPROPERTY()
	UCardDisplayWidget* CardDisplayWidget;
	UPROPERTY()
	UGameTitleMessageWidget* GameTitleMessageWidget;
	UPROPERTY()
	UAbilityPanelWidget* AbilityPanelWidget;
	UPROPERTY()
	UAbilitySelectWindowWidget* AbilitySelectWindowWidget;
	UPROPERTY()
	UUserWidget* DebugUIWidget;

	AStandardGameState* StandardGameState;
};

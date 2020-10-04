// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "CustomizableCardTypes.h"
#include "DogFightTypes.h"
#include "UIType.h"
#include "GameFramework/HUD.h"
#include "StandardHUD.generated.h"

class UGamePhaseMessageWidget;
class UOperationHintMessageWidget;
class UGameMessageWindowWidget;
class UGameRoundsTimelineWidget;
class UCardDisplayWidget;
class AStandardGameState;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStandardHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

public:

	/** Display specified hint message on screen. */
	void DisplayHintMessage(FName HintMessage);

	void HideOperationHintMessageWidget();

	void ShowGameMessage(FGameMessage Message);

	void InitializeTimelineDisplayTimeline();

	void SetCardDisplayInfoList(const TArray<FCardInstanceDisplayInfo>& CardInfoList);

	void ToggleCardDisplayWidgetVisibility(bool bVisible);

	void SetCardSelectionMode(ECardSelectionMode SelectionMode);

	FORCEINLINE UCardDisplayWidget* GetCardDisplayWidget() const { return CardDisplayWidget; }

	/** Switch the visibility of TimelineDisplayWidget. */
	void SetTimelineVisibility(bool bVisible);

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

protected:
	void DrawMiniMap();

	UFUNCTION()
	void OnGamePhaseChanged(const FName& NewGamePhase);

	UFUNCTION()
	void OnCountdownContentStringChanged();

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

	AStandardGameState* StandardGameState;
};

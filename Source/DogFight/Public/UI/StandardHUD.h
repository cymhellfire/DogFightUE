// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StandardHUD.generated.h"

class UGamePhaseMessageWidget;
class UOperationHintMessageWidget;
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

protected:
	virtual void BeginPlay() override;

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

protected:
	void DrawMiniMap();

	UFUNCTION()
	void OnGamePhaseChanged(const FName& NewGamePhase);

	UFUNCTION()
	void OnCountdownContentStringChanged();

	UGamePhaseMessageWidget* GamePhaseMessageWidget;
	UOperationHintMessageWidget* OperationHintMessageWidget;

	AStandardGameState* StandardGameState;
};

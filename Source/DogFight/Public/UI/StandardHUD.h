// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StandardHUD.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStandardHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

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

protected:
	void DrawMiniMap();
};

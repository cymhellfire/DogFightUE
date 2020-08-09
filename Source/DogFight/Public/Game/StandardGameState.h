// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "StandardMiniMapCapture.h"
#include "GameFramework/GameStateBase.h"
#include "StandardGameState.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStandardGameState : public AGameStateBase
{
	GENERATED_UCLASS_BODY()

public:
	/* Mini Map Capture Camera */
	TWeakObjectPtr<AStandardMiniMapCapture> MiniMapCamera;

	UPROPERTY(Category=Map, VisibleAnywhere, BlueprintReadOnly)
	/* World bounds for Mini Map and Camera movement. */
	FBox WorldBounds;
};

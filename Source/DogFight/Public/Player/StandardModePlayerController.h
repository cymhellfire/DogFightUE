// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StandardModePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStandardModePlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void ProcessPlayerInput(const float DeltaTime, const bool bGamePaused) override;
};

// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawnMovement.h"
#include "StandardModePlayerPawnMovement.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UStandardModePlayerPawnMovement : public USpectatorPawnMovement
{
	GENERATED_UCLASS_BODY()

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

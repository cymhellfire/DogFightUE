#pragma once

#include "GameFramework/SpectatorPawnMovement.h"
#include "TopDownStyleMovementComponent.generated.h"

UCLASS()
class DOGFIGHTGAME_API UTopDownStyleMovementComponent : public USpectatorPawnMovement
{
	GENERATED_BODY()
public:
	UTopDownStyleMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

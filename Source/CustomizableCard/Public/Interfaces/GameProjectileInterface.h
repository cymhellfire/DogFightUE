#pragma once

#include "CoreMinimal.h"
#include "GameProjectileInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UGameProjectileInterface : public  UInterface
{
	GENERATED_BODY()
};

/**
 * Interface that allows projectiles interact with card instruction.
 */
class IGameProjectileInterface
{
	GENERATED_BODY()

public:

	virtual void AdjustGravityScale(float NewGravityScale) {};

	virtual void SetInitialSpeed(float NewSpeed) {};

	virtual void SetMaxSpeed(float NewSpeed) {};

	virtual void LaunchAtDirection(const FVector& Direction) {};

	virtual void SetOwnerController(AController* NewController) {};

	virtual void SetOwnerCharacter(AActor* NewActor) {};
};

#pragma once

#include "CoreMinimal.h"
#include "GameBuffInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UGameBuffInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface that allows buffs interact with card instruction.
 */
class IGameBuffInterface
{
	GENERATED_BODY()

public:

	/** Set id of source player. */
	virtual void SetSourcePlayerController(AController* PlayerController) {};

	/** Handle the lifetime setup for gameplay buffs. */
	virtual void SetLifetime(float Lifetime) {};

	/** Set the target actor of this buff. */
	virtual void SetTargetActor(AActor* TargetActor) {};
};
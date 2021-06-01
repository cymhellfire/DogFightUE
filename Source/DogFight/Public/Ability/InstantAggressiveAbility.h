// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "UsingCardAbility.h"
#include "InstantAggressiveAbility.generated.h"

class ACardBase;

/**
 * Ability that can be cast aggressively.
 */
UCLASS(Blueprintable)
class DOGFIGHT_API UInstantAggressiveAbility : public UUsingCardAbility
{
	GENERATED_BODY()

public:
	UInstantAggressiveAbility(const FObjectInitializer& ObjectInitializer);
};

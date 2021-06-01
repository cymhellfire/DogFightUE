// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UsingCardAbility.h"
#include "TriggerablePassiveAbility.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable)
class DOGFIGHT_API UTriggerablePassiveAbility : public UUsingCardAbility
{
	GENERATED_BODY()

public:
	UTriggerablePassiveAbility(const FObjectInitializer& ObjectInitializer);

	virtual void RegisterAbility(AStandardPlayerState* OwnerPlayerState) override;
	virtual void UnregisterAbility() override;
protected:

	virtual void RegisterCondition();
	virtual void UnregisterCondition();
};

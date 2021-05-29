// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilityBase.h"
#include "PermanentPassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UPermanentPassiveAbility : public UAbilityBase
{
	GENERATED_BODY()

public:

	UPermanentPassiveAbility(const FObjectInitializer& ObjectInitializer);

	virtual void RegisterAbility(AStandardPlayerState* OwnerPlayerState) override;

	virtual void UnregisterAbility() override;

	virtual bool Activate() override;

	virtual void Deactivate();
};

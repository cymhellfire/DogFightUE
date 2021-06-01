// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Ability/TriggerablePassiveAbility.h"
#include "PassiveAbility_OnDead.generated.h"

/**
 * Passive ability that triggered when character dead.
 */
UCLASS(Blueprintable)
class DOGFIGHT_API UPassiveAbility_OnDead : public UTriggerablePassiveAbility
{
	GENERATED_BODY()

public:
	virtual bool IsAbilityUsable() const override;

protected:
	virtual void RegisterCondition() override;
	virtual void UnregisterCondition() override;

	UFUNCTION()
	void OnCharacterDead();
};

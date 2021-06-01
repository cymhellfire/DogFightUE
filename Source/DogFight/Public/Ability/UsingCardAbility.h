#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "UsingCardAbility.generated.h"

UCLASS(NotBlueprintable)
class UUsingCardAbility : public UAbilityBase
{
	GENERATED_BODY()
public:
	virtual bool Activate() override;

protected:
	/** Card to use when cast this ability. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	TSubclassOf<class ACardBase> AbilityCardClass;
};

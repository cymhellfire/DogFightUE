// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilityBase.h"
#include "InstantAggressiveAbility.generated.h"

class ACardBase;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInstantAggressiveAbility : public UAbilityBase
{
	GENERATED_BODY()

public:

	UInstantAggressiveAbility(const FObjectInitializer& ObjectInitializer);

	virtual bool Activate() override;
protected:
	/** Card to use when cast this ability. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	TSubclassOf<ACardBase> AbilityCardClass;
};

// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Ability/PermanentPassiveAbility.h"
#include "PermanentBuffAbility.generated.h"

class ABuffBase;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UPermanentBuffAbility : public UPermanentPassiveAbility
{
	GENERATED_BODY()

public:
	virtual void Activate() override;
	virtual void Deactivate() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	TSubclassOf<ABuffBase> BuffClass;

	UPROPERTY()
	ABuffBase* BuffInstance;
};

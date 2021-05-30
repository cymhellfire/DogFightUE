// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayAbilityPool.generated.h"

class UAbilityBase;

/**
 * 
 */
UCLASS(Blueprintable)
class DOGFIGHT_API UGameplayAbilityPool : public UObject
{
	GENERATED_BODY()

public:
	TArray<UAbilityBase*> GetRandomAbility(int32 Count);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilityPool")
	TArray<TSubclassOf<UAbilityBase>> AbilityClasses;
};

// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayCardPool.generated.h"

class ACardBase;

/**
 * This class holds all spawnable cards in current game.
 */
UCLASS(Blueprintable)
class DOGFIGHT_API UGameplayCardPool : public UObject
{
	GENERATED_BODY()

public:
	/** Get a new random card from pool. */
	ACardBase* GetRandomCard();

	/** Get a card by specified class index. */
	ACardBase* GetCardByIndex(int32 CardIndex);

	/** Add new card class into pool. */
	void RegisterCardClass(TSubclassOf<ACardBase> NewClass);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CardPool")
	TArray<TSubclassOf<ACardBase>> CardClassList;
};

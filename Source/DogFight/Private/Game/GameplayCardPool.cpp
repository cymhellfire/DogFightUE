// Dog Fight Game Code By CYM.


#include "GameplayCardPool.h"
#include "CardBase.h"
#include "DogFight.h"

ACardBase* UGameplayCardPool::GetRandomCard()
{
	if (CardClassList.Num() == 0)
		return nullptr;

	// Get a random card class as the result
	const int32 CardClassIndex = FMath::RandRange(0, CardClassList.Num() - 1);
	ACardBase* NewCard = GetWorld()->SpawnActor<ACardBase>(CardClassList[CardClassIndex]);

	return NewCard;
}

ACardBase* UGameplayCardPool::GetCardByIndex(int32 CardIndex)
{
	if (CardClassList.Num() == 0)
		return nullptr;

	if (CardIndex >= CardClassList.Num())
	{
		UE_LOG(LogDogFight, Error, TEXT("Index %d is not exist in CardPool"), CardIndex);
		return nullptr;
	}

	return GetWorld()->SpawnActor<ACardBase>(CardClassList[CardIndex]);
}

void UGameplayCardPool::RegisterCardClass(TSubclassOf<ACardBase> NewClass)
{
	if (!CardClassList.Contains(NewClass))
	{
		CardClassList.Add(NewClass);

		UE_LOG(LogDogFight, Log, TEXT("Add class %s into CardPool."), *NewClass->GetName())
	}
}

// Dog Fight Game Code By CYM.


#include "Game/GameplayAbilityPool.h"
#include "Ability/AbilityBase.h"

TArray<UAbilityBase*> UGameplayAbilityPool::GetRandomAbility(int32 Count)
{
	if (AbilityClasses.Num() == 0)
		return TArray<UAbilityBase*>();

	TArray<UAbilityBase*> Result;
	TArray<int32> CandidateIndex;
	for (int32 Index = 0; Index < AbilityClasses.Num(); ++Index)
	{
		CandidateIndex.Add(Index);
	}

	for (int32 Index = 0; Index < Count; ++Index)
	{
		const int32 AbilityIndex = FMath::RandRange(0, CandidateIndex.Num() - 1);

		Result.Add(NewObject<UAbilityBase>(this, AbilityClasses[CandidateIndex[AbilityIndex]]));
		CandidateIndex.RemoveAt(AbilityIndex);
		// Break if no ability left
		if (CandidateIndex.Num() == 0)
		{
			break;
		}
	}

	return Result;
}

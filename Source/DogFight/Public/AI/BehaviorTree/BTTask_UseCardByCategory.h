// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UseCardByCategory.generated.h"

/**
 * Use a card with given category flags to target character.
 */
UCLASS()
class DOGFIGHT_API UBTTask_UseCardByCategory : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="UseCard")
	bool bUseBlackboard;

	UPROPERTY(EditAnywhere, Category="UseCard", Meta=(Bitmask, BitmaskEnum="ECardCategoryFlags"))
	int32 CardCategory;

	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector TargetCharacterKey;
};

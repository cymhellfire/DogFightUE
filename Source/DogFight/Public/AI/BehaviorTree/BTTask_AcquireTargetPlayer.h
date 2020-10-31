// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_AcquireTargetPlayer.generated.h"

/**
 * Acquire a target player with given condition and record to blackboard.
 */
UCLASS()
class DOGFIGHT_API UBTTask_AcquireTargetPlayer : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category="AcquireTarget", meta=(Bitmask, BitmaskEnum = "EFindPlayerFlags"))
	int32 FindFlags;
};

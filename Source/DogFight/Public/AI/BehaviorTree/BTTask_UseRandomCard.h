// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UseRandomCard.generated.h"

/**
 * Use Random Card Node.
 * Use a random card without any rule.
 */
UCLASS()
class DOGFIGHT_API UBTTask_UseRandomCard : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

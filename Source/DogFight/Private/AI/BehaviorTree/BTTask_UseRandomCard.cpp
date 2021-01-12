// Dog Fight Game Code By CYM.


#include "AI/BehaviorTree/BTTask_UseRandomCard.h"
#include "AI/StandardModeAIController.h"

UBTTask_UseRandomCard::UBTTask_UseRandomCard(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Use A Random Card";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_UseRandomCard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController != nullptr)
	{
		if (AStandardModeAIController* StandardModeAIController = Cast<AStandardModeAIController>(AIController))
		{
			StandardModeAIController->UseRandomCard();
			return EBTNodeResult::InProgress;
		}
		else
		{
			UE_LOG(LogBehaviorTree, Error, TEXT("Failed to cast AIController to StandardModeAIController."));
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTask_UseRandomCard::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController != nullptr)
	{
		if (AStandardModeAIController* StandardModeAIController = Cast<AStandardModeAIController>(AIController))
		{
			// Check if the card is finished
			if (StandardModeAIController->GetCurrentState() != EStandardModeAIControllerState::UsingCard)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
		else
		{
			UE_LOG(LogBehaviorTree, Error, TEXT("Failed to cast AIController to StandardModeAIController."));
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}
	else
	{
		UE_LOG(LogBehaviorTree, Error, TEXT("No available AIController for Task %s"), *GetNodeName());
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed); 
	}
}


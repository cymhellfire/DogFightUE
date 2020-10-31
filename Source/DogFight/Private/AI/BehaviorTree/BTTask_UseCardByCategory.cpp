// Dog Fight Game Code By CYM.


#include "BTTask_UseCardByCategory.h"

#include "StandardModeAIController.h"
#include "StandardModePlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTask_UseCardByCategory::UBTTask_UseCardByCategory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Use Card By Category";
	bNotifyTick = true;

	// Only accept int32
	BlackboardKey.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_UseCardByCategory, BlackboardKey));

	TargetCharacterKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_UseCardByCategory, TargetCharacterKey), AStandardModePlayerCharacter::StaticClass());

	bUseBlackboard = true;
}

EBTNodeResult::Type UBTTask_UseCardByCategory::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController != nullptr)
	{
		const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

		if (AStandardModeAIController* StandardModeAIController = Cast<AStandardModeAIController>(AIController))
		{
			// Set target pawn
			UObject* ObjectValue = MyBlackboard->GetValueAsObject(TargetCharacterKey.SelectedKeyName);

			UE_LOG(LogDogFight, Log, TEXT("%s: Get %s equals %s [%d]"), *OwnerComp.GetAIOwner()->GetName(), *TargetCharacterKey.SelectedKeyName.ToString(), *(ObjectValue != nullptr ? ObjectValue->GetName() : FString(TEXT("Null"))), MyBlackboard);

			AStandardModePlayerCharacter* TargetCharacter = Cast<AStandardModePlayerCharacter>(ObjectValue);
			StandardModeAIController->SetTargetCharacter(TargetCharacter);

			if (bUseBlackboard)
			{
				const int32 KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Int>(BlackboardKey.GetSelectedKeyID());

				if (StandardModeAIController->UseCardByCategoryFlags(KeyValue))
				{
					return EBTNodeResult::InProgress;
				}
				else
				{
					return EBTNodeResult::Failed;
				}
			}
			else
			{
				if (StandardModeAIController->UseCardByCategoryFlags(CardCategory))
				{
					return EBTNodeResult::InProgress;
				}
				else
				{
					return EBTNodeResult::Failed;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTask_UseCardByCategory::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

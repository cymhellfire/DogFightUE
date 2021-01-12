// Dog Fight Game Code By CYM.


#include "AI/BehaviorTree/BTTask_AcquireTargetPlayer.h"
#include "Pawns/StandardModePlayerCharacter.h"
#include "AI/StandardModeAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTask_AcquireTargetPlayer::UBTTask_AcquireTargetPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Acquire Target Player";

	// Only accept object
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_AcquireTargetPlayer, BlackboardKey), AStandardModePlayerCharacter::StaticClass());
}

EBTNodeResult::Type UBTTask_AcquireTargetPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AStandardModeAIController* StandardModeAIController = Cast<AStandardModeAIController>(AIController))
	{
		if (AStandardModePlayerCharacter* TargetCharacter = StandardModeAIController->AcquireTargetPlayerCharacter(FindFlags))
		{
			UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
			MyBlackboard->SetValue<UBlackboardKeyType_Object>(BlackboardKey.SelectedKeyName, TargetCharacter);

			UE_LOG(LogDogFight, Log, TEXT("%s: Set %s to %s [%d]"), *OwnerComp.GetAIOwner()->GetName(), *BlackboardKey.SelectedKeyName.ToString(), *TargetCharacter->GetName(), MyBlackboard);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

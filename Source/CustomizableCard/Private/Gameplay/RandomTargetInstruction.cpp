// Dog Fight Game Code By CYM.


#include "RandomTargetInstruction.h"
#include "CardBase.h"
#include "GameTargetProviderInterface.h"

URandomTargetInstruction::URandomTargetInstruction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TargetType = ECardInstructionTargetType::Actor;
	TargetCount = 1;
	bIgnoreSelf = false;
}

void URandomTargetInstruction::Execute()
{
	switch(TargetType)
	{
	case ECardInstructionTargetType::Actor:
		GenerateActorTarget();
		break;
	case ECardInstructionTargetType::Position:
		GeneratePositionTarget();
		break;
	case ECardInstructionTargetType::Direction:
		GenerateDirectionTarget();
		break;
	default: ;
	}

	Finish();
}

void URandomTargetInstruction::GenerateActorTarget()
{
	AController* PlayerController = GetOwnerCard()->GetOwnerPlayerController();
	IGameTargetProviderInterface* Provider = Cast<IGameTargetProviderInterface>(PlayerController);
	if (Provider != nullptr)
	{
		for (int i = 0; i < TargetCount; ++i)
		{
			GetOwnerCard()->PushTargetInfo(Provider->RequestRandomActorTarget(bIgnoreSelf));
		}
	}
}

void URandomTargetInstruction::GeneratePositionTarget()
{
	AController* PlayerController = GetOwnerCard()->GetOwnerPlayerController();
	IGameTargetProviderInterface* Provider = Cast<IGameTargetProviderInterface>(PlayerController);
	if (Provider != nullptr)
	{
		for (int i = 0; i < TargetCount; ++i)
		{
			GetOwnerCard()->PushTargetInfo(Provider->RequestRandomPositionTarget());
		}
	}
}

void URandomTargetInstruction::GenerateDirectionTarget()
{
	AController* PlayerController = GetOwnerCard()->GetOwnerPlayerController();
	IGameTargetProviderInterface* Provider = Cast<IGameTargetProviderInterface>(PlayerController);
	if (Provider != nullptr)
	{
		for (int i = 0; i < TargetCount; ++i)
		{
			GetOwnerCard()->PushTargetInfo(Provider->RequestRandomDirectionTarget());
		}
	}
}

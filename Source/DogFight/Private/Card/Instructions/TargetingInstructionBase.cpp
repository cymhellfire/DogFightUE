// Dog Fight Game Code By CYM.


#include "Card/Instructions/TargetingInstructionBase.h"
#include "Card/CardBase.h"
#include "Actors/Interfaces/GameTargetProviderInterface.h"

UTargetingInstructionBase::UTargetingInstructionBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TargetType = ECardInstructionTargetType::Actor;
	TargetCount = 1;
}


void UTargetingInstructionBase::Execute()
{
	TargetAcquired = 0;
	
	AController* PlayerController = GetOwnerCard()->GetOwnerPlayerController();
	TargetProvider = Cast<IGameTargetProviderInterface>(PlayerController);
	if (TargetProvider != nullptr)
	{
		TargetProvider->GetTargetInfoAcquiredDelegate().AddDynamic(this, &UTargetingInstructionBase::OnTargetAcquired);

		RequestTarget();
	}
}

void UTargetingInstructionBase::RequestTarget()
{
	switch(TargetType)
	{
	case ECardInstructionTargetType::Actor:
		TargetProvider->RequestActorTarget();
        break;
	case ECardInstructionTargetType::Position:
		TargetProvider->RequestPositionTarget();
		break;
	case ECardInstructionTargetType::Direction:
		TargetProvider->RequestDirectionTarget();
		break;
	default: ;
	}
}

void UTargetingInstructionBase::OnTargetAcquired(FCardInstructionTargetInfo& TargetInfo)
{
	TargetAcquired++;

	// Send the TargetInfo to owner card
	GetOwnerCard()->PushTargetInfo(TargetInfo);

	// Remove the callback and finish this instruction if all targets are acquired
	if (TargetProvider != nullptr)
	{
		if (TargetAcquired >= TargetCount)
		{
			TargetProvider->GetTargetInfoAcquiredDelegate().RemoveDynamic(this, &UTargetingInstructionBase::OnTargetAcquired);

			Finish();
		}
		else
		{
			RequestTarget();
		}
	}
}

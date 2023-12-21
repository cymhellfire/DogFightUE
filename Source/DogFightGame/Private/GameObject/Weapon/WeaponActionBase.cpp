// Dog Fight Game Code By CYM.

#include "GameObject/Weapon/WeaponActionBase.h"

#include "Common/DogFightGameLog.h"
#include "DataAsset/WeaponActionDataAsset.h"
#include "GameObject/Weapon/WeaponActionTransitionBase.h"
#include "GameObject/Weapon/WeaponBase.h"
#include "Interface/ActionCharacterInterface/ActionCharacterInterface.h"

UWeaponActionBase::UWeaponActionBase()
{
	Performer = nullptr;
}

void UWeaponActionBase::InitActionData(UWeaponActionDataAsset* InData, IActionCharacterInterface* Owner)
{
	if (!IsValid(InData))
	{
		return;
	}

	ActionName = InData->Name;
	ActionDescription = InData->Description;
	bNeedTarget = InData->bNeedTarget;
	ActionRange = InData->Range;
	ActionMontage = InData->AnimMontage.IsValid() ? InData->AnimMontage.Get() : InData->AnimMontage.LoadSynchronous();

	Performer = Owner;
}

void UWeaponActionBase::AddTransition(EWeaponActionInput InInput, UWeaponActionBase* InAction)
{
	if (!IsValid(InAction))
	{
		return;
	}

	auto NewTransition = NewObject<UWeaponActionTransitionBase>(this);
	if (NewTransition)
	{
		NewTransition->InitTransition(OwnerWeapon, InInput, InAction);
		TransitionMap.Add(InInput, NewTransition);
	}
}

void UWeaponActionBase::SetActionTarget(const FWeaponActionTarget& InTarget)
{
	// Skip if target is unnecessary
	if (!bNeedTarget)
	{
		return;
	}

	ActionTarget = InTarget;
}

void UWeaponActionBase::Execute()
{
	if (!Performer)
		return;

	UE_LOG(LogDogFightGame, Log, TEXT("[WeaponActionBase] Execute: %s"), *ActionName.ToString());

	const auto TargetResult = CheckDistance();
	switch(TargetResult)
	{
	case EDistanceCheckResult::OutOfRange:
		GoToTarget();
		break;
	case EDistanceCheckResult::InRange:
		PerformAction();
		break;
	case EDistanceCheckResult::Invalid:
	default:
		UE_LOG(LogDogFightGame, Error, TEXT("[UWeaponActionBase] Execute with invalid target."));
	}
}

UWeaponActionBase::EDistanceCheckResult UWeaponActionBase::CheckDistance()
{
	// It's always consider as in range when no target required.
	if (!bNeedTarget)
	{
		return EDistanceCheckResult::InRange;
	}

	if (ActionTarget.IsSet())
	{
		float Distance;
		switch(ActionTarget->TargetType)
		{
		case EWeaponActionTargetType::WATT_Actor:
			Distance = Performer->GetDistanceFrom(ActionTarget->GetActorTarget());
			break;
		case EWeaponActionTargetType::WATT_Location:
			Distance = Performer->GetDistanceFrom(ActionTarget->GetLocationTarget());
			break;
		case EWeaponActionTargetType::WATT_None:
		default:
			// Invalid target info
			return EDistanceCheckResult::Invalid;
		}

		return (Distance <= ActionRange) ? EDistanceCheckResult::InRange : EDistanceCheckResult::OutOfRange;
	}

	// It's invalid when a target is required but no one specified
	return EDistanceCheckResult::Invalid;
}

void UWeaponActionBase::GoToTarget()
{
	if (ActionTarget.IsSet())
	{
		FVector TargetLoc;
		switch(ActionTarget->TargetType)
		{
		case EWeaponActionTargetType::WATT_Actor:
			TargetLoc = ActionTarget->GetActorTarget()->GetActorLocation();
			break;
		case EWeaponActionTargetType::WATT_Location:
			TargetLoc = ActionTarget->GetLocationTarget();
			break;
		case EWeaponActionTargetType::WATT_None:
		default:
			;
		}

		Performer->OnReachActionDistance.AddUObject(this, &UWeaponActionBase::OnReachActionDistance);
		Performer->MoveToTarget(TargetLoc, ActionRange);
	}
}

void UWeaponActionBase::OnReachActionDistance()
{
	if (Performer)
	{
		Performer->OnReachActionDistance.RemoveAll(this);

		UE_LOG(LogDogFightGame, Log, TEXT("[WeaponActionBase] Reach distance: %s"), *ActionName.ToString());

		PerformAction();
	}
}

void UWeaponActionBase::PerformAction()
{
	const float AnimDuration = PlayActionMontage();
	if (AnimDuration > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(ActionTimerHandler, this, &UWeaponActionBase::OnActionMontageFinished, AnimDuration);
	}
	else
	{
		OnActionMontageFinished();
	}
}

UWeaponActionTransitionBase* UWeaponActionBase::GetTransitionByInput(EWeaponActionInput Input) const
{
	if (auto TransitionPtr = TransitionMap.Find(Input))
	{
		return *TransitionPtr;
	}
	return nullptr;
}

float UWeaponActionBase::PlayActionMontage()
{
	if (!IsValid(ActionMontage))
	{
		return 0.f;
	}

	return Performer->PlayActionAnimation(ActionMontage);
}

void UWeaponActionBase::OnActionMontageFinished()
{
	if (ActionTimerHandler.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ActionTimerHandler);
	}

	FinishAction();
}

void UWeaponActionBase::FinishAction()
{
	UE_LOG(LogDogFightGame, Log, TEXT("[WeaponActionBase] Finish: %s"), *ActionName.ToString());

	//ConsumeInput();
	OnActionFinished.Broadcast(this);
}

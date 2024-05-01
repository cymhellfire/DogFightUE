// Dog Fight Game Code By CYM.


#include "Object/TargetingWeaponAction.h"

#include "Common/ActionGameWeaponLog.h"
#include "DataAsset/TargetingWeaponActionDataAsset.h"
#include "Interface/ActionCharacterInterface.h"

bool UTargetingWeaponAction::InitActionData(UWeaponActionDataAsset* InData, IActionCharacterInterface* InOwner)
{
	if (!Super::InitActionData(InData, InOwner))
	{
		return false;
	}

	if (auto TargetingData = Cast<UTargetingWeaponActionDataAsset>(InData))
	{
		bWarpingToTarget = TargetingData->bWarpingToTarget;
		WarpingTargetName = TargetingData->WarpingTargetName;
		ActionRange = TargetingData->Range;
	}
	else
	{
		ensureMsgf(false, TEXT("[TargetingWeaponAction] Weapon action data %s is not derived from UTargetingWeaponActionDataAsset class.")
			, *InData->GetName());
	}

	return true;
}

void UTargetingWeaponAction::SetActionTarget(const FWeaponActionTarget& InTarget)
{
	ActionTarget = InTarget;
}

bool UTargetingWeaponAction::Execute()
{
	if (!Super::Execute())
	{
		return false;
	}

	// Check action distance
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
		UE_LOG(LogActionGameWeapon, Error, TEXT("[UWeaponActionBase] Execute with invalid target."));
	}
	return true;
}

UTargetingWeaponAction::EDistanceCheckResult UTargetingWeaponAction::CheckDistance()
{
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

void UTargetingWeaponAction::GoToTarget()
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

		Performer->OnReachActionDistance.AddUObject(this, &UTargetingWeaponAction::OnReachActionDistance);
		Performer->MoveToTarget(TargetLoc, ActionRange);
	}
}

void UTargetingWeaponAction::OnReachActionDistance()
{
	if (Performer)
	{
		Performer->OnReachActionDistance.RemoveAll(this);

		UE_LOG(LogActionGameWeapon, Log, TEXT("[WeaponActionBase] Reach distance: %s"), *ActionName.ToString());

		PerformAction();
	}
}

float UTargetingWeaponAction::PlayActionMontage()
{
	if (IsValid(ActionMontage))
	{
		TOptional<FVector> WarpingPos;
		if (ActionTarget.IsSet())
		{
			WarpingPos = ActionTarget->AsLocation();
		}

		if (bWarpingToTarget && WarpingPos.IsSet() && WarpingTargetName.IsValid())
		{
			return Performer->PlayActionAnimationWithWarping(ActionMontage, WarpingTargetName, WarpingPos.GetValue());
		}
	}

	return Super::PlayActionMontage();
}

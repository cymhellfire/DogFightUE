// Dog Fight Game Code By CYM.

#include "GameObject/Weapon/WeaponBase.h"

#include "Common/DogFightGameLog.h"
#include "DataAsset/WeaponActionDataAsset.h"
#include "DataAsset/WeaponDataAsset.h"
#include "GameObject/Weapon/WeaponActionBase.h"
#include "GameObject/Weapon/WeaponActionTransitionBase.h"

UWeaponBase::UWeaponBase()
{
	bFiredInputFinished = false;
	CurrentAction = DefaultAction = nullptr;
}

void UWeaponBase::InitWithWeaponData(UWeaponDataAsset* InWeaponData)
{
	if (!IsValid(InWeaponData))
	{
		return;
	}

	TMap<FString, UWeaponActionBase*> ActionMap;
	for (auto ActionData : InWeaponData->WeaponActionList)
	{
		if (auto NewAction = AddWeaponAction(ActionData))
		{
			ActionMap.Add(ActionData->Name.ToString(), NewAction);
		}
	}

	// Setup transition
	for (auto ActionData : InWeaponData->WeaponActionList)
	{
		for (auto Setting : ActionData->TransitionSettings)
		{
			if (auto ActionPtr = ActionMap.Find(ActionData->Name.ToString()))
			{
				auto Action = *ActionPtr;
				if (auto TargetPtr = ActionMap.Find(Setting.WeaponAction->Name.ToString()))
				{
					Action->AddTransition(Setting.InputType, *TargetPtr);
				}
			}
		}
	}

	// Set default action
	if (IsValid(InWeaponData->DefaultAction))
	{
		if (auto ActionPtr = ActionMap.Find(InWeaponData->DefaultAction->Name.ToString()))
		{
			DefaultAction = *ActionPtr;
			PerformAction(*ActionPtr);
		}
	}
}

UWeaponActionBase* UWeaponBase::AddWeaponAction(UWeaponActionDataAsset* InActionData)
{
	if (!IsValid(InActionData))
	{
		return nullptr;
	}

	// Create new weapon action
	auto NewAction = NewObject<UWeaponActionBase>(this);
	if (IsValid(NewAction))
	{
		NewAction->SetOwnerWeapon(this);
		NewAction->InitActionData(InActionData, OwnerCharacter);
		WeaponActions.AddUnique(NewAction);
	}

	return NewAction;
}

void UWeaponBase::EnqueueInput(EWeaponActionInput NewInput, const FWeaponActionTarget& InTarget)
{
	InputQueue.Emplace(FWeaponActionInfo(NewInput, InTarget));

	bFiredInputFinished = false;
}

FWeaponActionInfo UWeaponBase::DequeueInput()
{
	if (InputQueue.Num() > 0)
	{
		auto Result = InputQueue[0];
		InputQueue.RemoveAt(0);
		return Result;
	}

	return FWeaponActionInfo();
}

void UWeaponBase::StartInputQueue()
{
	if (!IsValid(CurrentAction))
	{
		UE_LOG(LogDogFightGame, Error, TEXT("[UWeaponBase] Cannot start input queue due to current action is missing."));
		return;
	}

	ConsumeInput();
}

void UWeaponBase::ConsumeInput()
{
	auto NextInput = DequeueInput();
	if (!NextInput.IsValid())
	{
		CheckInputQueue();
		return;
	}

	// Transfer to next action
	if (auto Transition = CurrentAction->GetTransitionByInput(NextInput.Input))
	{
		Transition->DoTransition(NextInput.Target);
	}
	else
	{
		ResetWeapon();
	}
}

void UWeaponBase::PerformAction(UWeaponActionBase* InAction, const FWeaponActionTarget& InTarget)
{
	if (!IsValid(InAction))
	{
		return;
	}

	CurrentAction = InAction;
	CurrentAction->OnActionFinished.AddUObject(this, &UWeaponBase::OnActionFinished);
	CurrentAction->SetActionTarget(InTarget);
	CurrentAction->Execute();
}

void UWeaponBase::OnActionFinished(UWeaponActionBase* InAction)
{
	if (!IsValid(InAction))
		return;

	InAction->OnActionFinished.RemoveAll(this);
	ConsumeInput();
}

void UWeaponBase::ResetWeapon()
{
	InputQueue.Empty();

	// Set default action
	if (IsValid(DefaultAction))
	{
		if (DefaultAction != CurrentAction)
		{
			PerformAction(DefaultAction);
		}
	}
	else
	{
		UE_LOG(LogDogFightGame, Warning, TEXT("[UWeaponBase] No default action for weapon [%s] to reset."), *GetName())
	}

	CheckInputQueue();
}

void UWeaponBase::CheckInputQueue()
{
	if (!bFiredInputFinished && InputQueue.IsEmpty())
	{
		OnWeaponInputFinished.Broadcast(this);
		bFiredInputFinished = true;
	}
}

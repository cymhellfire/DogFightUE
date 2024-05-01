// Dog Fight Game Code By CYM.

#include "Object/WeaponActionBase.h"

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Animation/Notify/AnimNotifyState_AttackDetect.h"
#include "Common/ActionGameWeaponLog.h"
#include "DataAsset/WeaponActionDataAsset.h"
#include "Object/WeaponActionTransitionBase.h"
#include "Object/WeaponBase.h"
#include "Interface/ActionCharacterInterface.h"

UWeaponActionBase::UWeaponActionBase()
{
	Performer = nullptr;
}

bool UWeaponActionBase::InitActionData(UWeaponActionDataAsset* InData, IActionCharacterInterface* Owner)
{
	if (!IsValid(InData))
	{
		return false;
	}

	ActionName = InData->Name;
	ActionDescription = InData->Description;
	ActionMontage = InData->AnimMontage.IsValid() ? InData->AnimMontage.Get() : InData->AnimMontage.LoadSynchronous();

	Performer = Owner;

	return true;
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

bool UWeaponActionBase::Execute()
{
	if (!Performer)
		return false;

	UE_LOG(LogActionGameWeapon, Log, TEXT("[WeaponActionBase] Execute: %s"), *ActionName.ToString());
	return true;
}

void UWeaponActionBase::PerformAction()
{
	BindAnimNotifies();
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
	UE_LOG(LogActionGameWeapon, Log, TEXT("[WeaponActionBase] Finish: %s"), *ActionName.ToString());

	//ConsumeInput();
	UnbindAnimNotifies();
	OnActionFinished.Broadcast(this);
}

void UWeaponActionBase::BindAnimNotifies()
{
	if (!IsValid(ActionMontage))
	{
		return;
	}

	// Iterate through notify list
	for (auto& Notify : ActionMontage->Notifies)
	{
		if (auto NotifyState = Notify.NotifyStateClass.Get())
		{
			if (auto AttackDetectState = Cast<UAnimNotifyState_AttackDetect>(NotifyState))
			{
				AttackDetectState->OnAttackDetectStateChanged.AddUObject(this, &UWeaponActionBase::OnAttackDetectStateChanged);
			}
		}
	}
}

void UWeaponActionBase::UnbindAnimNotifies()
{
	if (!IsValid(ActionMontage))
	{
		return;
	}

	// Iterate through notify list
	for (auto& Notify : ActionMontage->Notifies)
	{
		if (auto NotifyState = Notify.NotifyStateClass.Get())
		{
			if (auto AttackDetectState = Cast<UAnimNotifyState_AttackDetect>(NotifyState))
			{
				AttackDetectState->OnAttackDetectStateChanged.RemoveAll(this);
			}
		}
	}
}

void UWeaponActionBase::OnAttackDetectStateChanged(bool bEnable)
{
	UE_LOG(LogActionGameWeapon, Log, TEXT("[WeaponActionBase] Attack detect: %s"), bEnable ? TEXT("on") : TEXT("off"));

	// Let weapon start collision detecting
	if (IsValid(OwnerWeapon))
	{
		OwnerWeapon->SetAttackDetectEnable(bEnable);
	}
}

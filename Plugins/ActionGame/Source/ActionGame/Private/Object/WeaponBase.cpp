// Dog Fight Game Code By CYM.

#include "Object/WeaponBase.h"

#include "Common/ActionGameWeaponLog.h"
#include "DataAsset/WeaponActionDataAsset.h"
#include "DataAsset/WeaponDataAsset.h"
#include "Actor/WeaponModelBase.h"
#include "FunctionLibrary/WeaponDataFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Object/WeaponActionBase.h"
#include "Object/WeaponActionTransitionBase.h"
#include "Interface/ActionCharacterInterface.h"

UWeaponBase::UWeaponBase()
{
	bFiredInputFinished = false;
	CurrentAction = DefaultAction = nullptr;
}

void UWeaponBase::BeginDestroy()
{
	// Destroy weapon model
	DestroyWeaponActor();

	UObject::BeginDestroy();
}

void UWeaponBase::InitWithWeaponData(UWeaponDataAsset* InWeaponData)
{
	if (!IsValid(InWeaponData))
	{
		return;
	}

	// Validate class
	if (InWeaponData->WeaponClass != GetClass())
	{
		UE_LOG(LogActionGameWeapon, Error, TEXT("[WeaponBase] Weapon class mismatch with weapon data of %s."),
			*InWeaponData->WeaponName.ToString());
		return;
	}

	ParentSocket = InWeaponData->ParentSocket;
	WeaponModelClass = InWeaponData->WeaponModelClass;
	SpawnWeaponActor();

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
	auto NewAction = NewObject<UWeaponActionBase>(this, InActionData->ActionClass.LoadSynchronous());
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
		UE_LOG(LogActionGameWeapon, Error, TEXT("[UWeaponBase] Cannot start input queue due to current action is missing."));
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

void UWeaponBase::SpawnWeaponActor()
{
	if (WeaponModelClass.IsNull())
	{
		UE_LOG(LogActionGameWeapon, Error, TEXT("[WeaponBase] No weapon actor class specified."));
		return;
	}

	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogActionGameWeapon, Error, TEXT("[WeaponBase] No owner for weapon."));
		return;
	}
	auto Owner = OwnerCharacter->AsCharacter();

	// Destroy existing actor
	DestroyWeaponActor();

	// Get the actual class and spawn actor
	const auto ActorClass = WeaponModelClass.IsValid() ? WeaponModelClass.Get() : WeaponModelClass.LoadSynchronous();
	const auto SpawnTrans = Owner->GetActorTransform();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Owner;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	WeaponModel = Cast<AWeaponModelBase>(GetWorld()->SpawnActor(ActorClass, &SpawnTrans, SpawnParameters));

	// Attach weapon actor to character
	if (WeaponModel.IsValid())
	{
		if(auto MeshComponent = Owner->GetMesh())
		{
			WeaponModel->AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ParentSocket);
		}

		// Record collision detecting component
		WeaponModel->SetOwnerCharacter(Owner);

		// Listen to hit event
		WeaponModel->HitEvent.AddUObject(this, &UWeaponBase::OnHitTarget);
	}
}

void UWeaponBase::DestroyWeaponActor()
{
	if (!WeaponModel.IsValid())
	{
		return;
	}

	WeaponModel->HitEvent.RemoveAll(this);

	WeaponModel->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	WeaponModel->Destroy();
	WeaponModel.Reset();
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
		UE_LOG(LogActionGameWeapon, Warning, TEXT("[WeaponBase] No default action for weapon [%s] to reset."), *GetName())
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

void UWeaponBase::SetAttackDetectEnable(bool bEnable)
{
	if (WeaponModel.IsValid())
	{
		WeaponModel->SetAttackDetectEnable(bEnable);
	}
	else
	{
		UE_LOG(LogActionGameWeapon, Error, TEXT("[WeaponBase] No weapon model spawned for weapon %s"), *GetName());
	}
}

void UWeaponBase::OnHitTarget(AActor* TargetActor, UPrimitiveComponent* TargetComponent, const FHitResult& HitResult)
{
	UE_LOG(LogActionGameWeapon, Log, TEXT("[WeaponBase] Attacked %s"), *TargetActor->GetName());
}

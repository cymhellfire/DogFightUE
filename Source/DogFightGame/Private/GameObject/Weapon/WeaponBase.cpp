// Dog Fight Game Code By CYM.

#include "GameObject/Weapon/WeaponBase.h"

#include "Common/DogFightGameLog.h"
#include "DataAsset/WeaponActionDataAsset.h"
#include "DataAsset/WeaponDataAsset.h"
#include "FunctionLibrary/CommonGameplayFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "GameObject/Weapon/WeaponActionBase.h"
#include "GameObject/Weapon/WeaponActionTransitionBase.h"
#include "Interface/ActionCharacterInterface/ActionCharacterInterface.h"

UWeaponBase::UWeaponBase()
{
	bFiredInputFinished = false;
	bAttackDetecting = true;
	CurrentAction = DefaultAction = nullptr;
}

void UWeaponBase::InitWithWeaponData(UWeaponDataAsset* InWeaponData)
{
	if (!IsValid(InWeaponData))
	{
		return;
	}

	ParentSocket = InWeaponData->ParentSocket;
	AttackDetectComponent = InWeaponData->AttackDetectComponent;
	WeaponActorClass = InWeaponData->WeaponActorClass;
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

void UWeaponBase::SpawnWeaponActor()
{
	if (WeaponActorClass.IsNull())
	{
		UE_LOG(LogDogFightGame, Error, TEXT("[WeaponBase] No weapon actor class specified."));
		return;
	}

	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogDogFightGame, Error, TEXT("[WeaponBase] No owner for weapon."));
		return;
	}
	auto Owner = OwnerCharacter->AsCharacter();

	// Destroy existing actor
	DestroyWeaponActor();

	// Get the actual class and spawn actor
	const auto ActorClass = WeaponActorClass.IsValid() ? WeaponActorClass.Get() : WeaponActorClass.LoadSynchronous();
	const auto SpawnTrans = Owner->GetActorTransform();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Owner;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	WeaponActor = GetWorld()->SpawnActor(ActorClass, &SpawnTrans, SpawnParameters);

	// Attach weapon actor to character
	if (WeaponActor.IsValid())
	{
		if(auto MeshComponent = Owner->GetMesh())
		{
			WeaponActor->AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ParentSocket);
		}

		// Record collision detecting component
		TInlineComponentArray<UPrimitiveComponent*> AttackDetectCandidates;
		WeaponActor->GetComponents<UPrimitiveComponent>(AttackDetectCandidates);
		for (auto Candidate : AttackDetectCandidates)
		{
			if (Candidate->GetFName() == AttackDetectComponent)
			{
				CachedCollisionComponent = Candidate;
				break;
			}
		}

		if (CachedCollisionComponent.IsValid())
		{
			CachedCollisionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &UWeaponBase::OnAttackDetectingOverlapped);
		}
		else
		{
			UE_LOG(LogDogFightGame, Error, TEXT("[WeaponBase] No attack detecting component matched name %s."),
				*AttackDetectComponent.ToString());
		}

		// Turn off detecting as default
		SetAttackDetectEnable(false);
	}
}

void UWeaponBase::DestroyWeaponActor()
{
	if (!WeaponActor.IsValid())
	{
		return;
	}

	CachedCollisionComponent->OnComponentBeginOverlap.RemoveDynamic(this, &UWeaponBase::OnAttackDetectingOverlapped);
	CachedCollisionComponent.Reset();
	WeaponActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	WeaponActor->Destroy();
	WeaponActor.Reset();
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

void UWeaponBase::SetAttackDetectEnable(bool bEnable)
{
	if (bAttackDetecting == bEnable)
	{
		return;
	}

	// Switch the collision component
	if (CachedCollisionComponent.IsValid())
	{
		CachedCollisionComponent->SetCollisionEnabled(bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
	bAttackDetecting = bEnable;
}

void UWeaponBase::OnAttackDetectingOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Skip character itself
	if (OtherActor == OwnerCharacter->AsCharacter())
	{
		return;
	}

	UE_LOG(LogDogFightGame, Log, TEXT("[WeaponBase] Attacked %s"), *OtherActor->GetName());

	UCommonGameplayFunctionLibrary::DamageActor(this, 1, OtherActor, 5, OwnerCharacter->AsCharacter());
}

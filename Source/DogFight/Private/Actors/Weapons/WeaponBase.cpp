// Dog Fight Game Code By CYM.


#include "Actors/Weapons/WeaponBase.h"
#include "Actors/Weapons/WeaponActionBase.h"
#include "Actors/Weapons/WeaponMeshActor.h"

// Sets default values
UWeaponBase::UWeaponBase()
{
	bEquipped = false;
	bAutoConsumeInput = false;
	BaseDamage = 5.f;
}

void UWeaponBase::SetWeaponOwner(ACharacter* NewOwner)
{
	if (NewOwner == OwnerCharacter)
		return;

	OwnerCharacter = NewOwner;
}

void UWeaponBase::Equip()
{
	if (bEquipped)
	{
		return;
	}

	if (!OwnerCharacter)
	{
		UE_LOG(LogDogFight, Error, TEXT("[Weapon] Invalid owner for equipping."));
		return;
	}

	// Get character mesh component for attachment
	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
	if (!MeshComponent)
	{
		UE_LOG(LogDogFight, Error, TEXT("[Weapon] Owner doesn't have valid MeshComponent."))
		return;
	}

	// Play equip montage
	if (IsValid(EquipAnimMontage))
	{
		UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance();
		const float EquipDuration = AnimInstance->Montage_Play(EquipAnimMontage);
		// Set the equip callback
		GetWorld()->GetTimerManager().SetTimer(WeaponEquipTimerHandle, this, &UWeaponBase::OnWeaponEquipped, EquipDuration);
	}
	else
	{
		OnWeaponEquipped();
	}

	// Enqueue all weapon mesh
	for (FWeaponMeshSetting WeaponMeshSetting : MeshSettings)
	{
		// Weapon mesh actor must have a valid parent socket name
		if (!WeaponMeshSetting.ParentSocketName.IsNone())
		{
			SpawningInfos.Add(FWeaponSpawningInfo(WeaponMeshSetting));
		}
	}

	CurrentWeaponState = WS_Equipping;
}

void UWeaponBase::UnEquip()
{
	if (!bEquipped)
	{
		return;
	}

	// Get character mesh component for attachment
	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
	if (!MeshComponent)
	{
		UE_LOG(LogDogFight, Error, TEXT("[Weapon] Owner doesn't have valid MeshComponent."))
		return;
	}

	// Play UnEquip animation
	if (IsValid(UnEquipAnimMontage))
	{
		UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance();
		const float UnEquipDuration = AnimInstance->Montage_Play(UnEquipAnimMontage);
		// Set the equip callback
		GetWorld()->GetTimerManager().SetTimer(WeaponUnEquipTimerHandle, this, &UWeaponBase::OnWeaponUnEquipped, UnEquipDuration);
	}
	else
	{
		OnWeaponUnEquipped();
	}

	CurrentWeaponState = WS_UnEquipping;
}

void UWeaponBase::EnqueueWeaponInput(EWeaponActionInput NewInput)
{
	// Skip None input
	if (NewInput == EWeaponActionInput::WAI_None)
	{
		return;
	}

	WeaponInputQueue.Enqueue(NewInput);

	// Consume input if weapon is not in action currently
	if (CurrentWeaponState == WS_None)
	{
		ConsumeNextInput();
	}
}

EWeaponActionInput UWeaponBase::DequeueWeaponInput()
{
	EWeaponActionInput Result;
	if (WeaponInputQueue.Dequeue(Result))
	{
		return Result;
	}

	// Return None if no input in queue
	return EWeaponActionInput::WAI_None;
}

void UWeaponBase::ResetWeaponAction()
{
	if (!WeaponInputQueue.IsEmpty())
	{
		WeaponInputQueue.Empty();
	}

	if (!InitialActionName.IsNone())
	{
		if (WeaponActionMap.Contains(InitialActionName))
		{
			StartWeaponAction(WeaponActionMap[InitialActionName]);
		}
	}
}

AWeaponMeshActor* UWeaponBase::GetWeaponMeshByParentSocket(FName SocketName)
{
	if (!WeaponMeshActorMap.Contains(SocketName))
	{
		return nullptr;
	}

	return WeaponMeshActorMap[SocketName];
}

void UWeaponBase::Tick(float DeltaTime)
{
	switch (CurrentWeaponState)
	{
	case WS_Equipping:
		UpdateSpawningQueue(DeltaTime);
		break;
	case WS_UnEquipping:
		UpdateDestroyQueue(DeltaTime);
		break;
	default:
		break;
	}
}

void UWeaponBase::InitialWeaponActions()
{
	for (auto ActionClass : WeaponActionClasses)
	{
		UWeaponActionBase* NewWeaponAction = NewObject<UWeaponActionBase>(this, ActionClass);
		if (!WeaponActionMap.Contains(NewWeaponAction->WeaponActionName))
		{
			NewWeaponAction->SetOwnerWeapon(this);
			WeaponActionMap.Add(NewWeaponAction->WeaponActionName, NewWeaponAction);
		}
		else
		{
			NewWeaponAction->ConditionalBeginDestroy();
		}
	}

	// Set current weapon action
	if (WeaponActionMap.Contains(InitialActionName))
	{
		CurrentAction = WeaponActionMap[InitialActionName];
	}
	else
	{
		UE_LOG(LogDogFight, Error, TEXT("[Weapon] Invalid initial weapon action %s"), *InitialActionName.ToString());
	}
}

void UWeaponBase::UpdateSpawningQueue(float DeltaTime)
{
	TArray<int32> FinishedIndex;
	for (int32 Index = 0; Index < SpawningInfos.Num(); ++Index)
	{
		SpawningInfos[Index].SpawnCountdown -= DeltaTime;
		if (SpawningInfos[Index].SpawnCountdown <= 0.f)
		{
			SpawnWeaponActor(SpawningInfos[Index]);

			FinishedIndex.Add(Index);
		}
	}

	// Remove finished items
	for (int32 Index = SpawningInfos.Num() - 1; Index >= 0; --Index)
	{
		if (FinishedIndex.Contains(Index))
		{
			SpawningInfos.RemoveAt(Index);
		}
	}
}

void UWeaponBase::UpdateDestroyQueue(float DeltaTime)
{
	TArray<int32> FinishedIndex;
	for (int32 Index = 0; Index < DestroyInfos.Num(); ++Index)
	{
		DestroyInfos[Index].DestroyCountdown -= DeltaTime;
		if (DestroyInfos[Index].DestroyCountdown <= 0.f)
		{
			// Destroy weapon actor
			DestroyInfos[Index].WeaponActor->Destroy();

			FinishedIndex.Add(Index);
		}
	}

	// Remove finished items
	for (int32 Index = DestroyInfos.Num() - 1; Index >= 0; --Index)
	{
		if (FinishedIndex.Contains(Index))
		{
			DestroyInfos.RemoveAt(Index);
		}
	}
}

void UWeaponBase::SpawnWeaponActor(FWeaponSpawningInfo SpawnSettings)
{
	// Check duplicate parent socket
	if (WeaponMeshActorMap.Contains(SpawnSettings.TargetSocketName))
	{
		UE_LOG(LogDogFight, Error, TEXT("[WeaponBase] Multiple actors spawns to same socket %s."), *SpawnSettings.TargetSocketName.ToString());
		return;
	}

	const FAttachmentTransformRules WeaponAttachRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();

	AWeaponMeshActor* NewMeshActor = GetWorld()->SpawnActor<AWeaponMeshActor>(SpawnSettings.WeaponMeshClass);
	NewMeshActor->AttachToComponent(MeshComponent, WeaponAttachRules, SpawnSettings.TargetSocketName);
	NewMeshActor->SetWeaponMeshOwner(OwnerCharacter);

	// Record new actor to array
	WeaponMeshActorMap.Add(SpawnSettings.TargetSocketName, NewMeshActor);

	// Create destroy info
	DestroyInfos.Add(FWeaponDestroyInfo{NewMeshActor, SpawnSettings.DestroyDelay});
}

void UWeaponBase::StartWeaponAction(UWeaponActionBase* ActionToStart)
{
	if (!IsValid(ActionToStart))
	{
		UE_LOG(LogDogFight, Warning, TEXT("[Weapon] Invalid weapon action to start."));
		return;
	}

	CurrentAction = ActionToStart;
	CurrentAction->EnterAction();

	// Register callback for non-initial action
	if (CurrentAction->WeaponActionName != InitialActionName)
	{
		CurrentAction->OnWeaponActionFinished.AddDynamic(this, &UWeaponBase::OnWeaponActionFinished);
		CurrentWeaponState = WS_InAction;
	}
	else
	{
		CurrentWeaponState =  WS_None;
	}
}

void UWeaponBase::ConsumeNextInput()
{
	// Dequeue input and check transition
	const EWeaponActionInput NextInput = DequeueWeaponInput();
	FName NextActionName = CurrentAction->GetNextActionWithInput(NextInput);

	// Correct ActionName to initial if no valid action transfer to
	if (NextActionName == NAME_None)
	{
		NextActionName = InitialActionName;
	}

	if (WeaponActionMap.Contains(NextActionName))
	{
		StartWeaponAction(WeaponActionMap[NextActionName]);
	}
	else
	{
		StartWeaponAction(WeaponActionMap[InitialActionName]);
	}
}

void UWeaponBase::OnWeaponEquipped()
{
	GetWorld()->GetTimerManager().ClearTimer(WeaponEquipTimerHandle);

	CurrentWeaponState = WS_None;
	bEquipped = true;

	InitialWeaponActions();

	// Trigger callback
	OnWeaponEquippedEvent.Broadcast();
}

void UWeaponBase::OnWeaponUnEquipped()
{
	GetWorld()->GetTimerManager().ClearTimer(WeaponUnEquipTimerHandle);

	WeaponMeshActorMap.Empty();

	CurrentWeaponState = WS_None;
	bEquipped = false;

	// Trigger callback
	OnWeaponUnEquippedEvent.Broadcast();
}

void UWeaponBase::OnWeaponActionFinished()
{
	// Unregister callback function
	if (IsValid(CurrentAction))
	{
		CurrentAction->OnWeaponActionFinished.RemoveDynamic(this, &UWeaponBase::OnWeaponActionFinished);
	}

	// Trigger callback
	OnWeaponActionFinishedEvent.Broadcast();

	// Consume next input if set
	if (bAutoConsumeInput)
	{
		ConsumeNextInput();
	}
	else
	{
		CurrentWeaponState = WS_None;
	}
}

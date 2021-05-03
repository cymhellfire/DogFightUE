// Dog Fight Game Code By CYM.


#include "Actors/Weapons/WeaponActionBase.h"

#include "Actors/Components/ReceiveDamageComponent.h"
#include "Actors/Interfaces/WeaponCarrierInterface.h"
#include "Actors/Weapons/WeaponBase.h"
#include "Actors/Weapons/WeaponDisplayRelative.h"
#include "Actors/Weapons/WeaponMeshActor.h"
#include "Animation/AnimNotify_InvincibleFrame.h"
#include "Animation/AnimNotify_SwitchHitDetect.h"
#include "Game/StandardGameMode.h"
#include "Pawns/StandardModePlayerCharacter.h"

UWeaponActionBase::UWeaponActionBase()
{
	ActionDistance = 50.f;
}

void UWeaponActionBase::EnterAction()
{
	// Check distance from target
	AActor* WeaponTarget = nullptr;
	ACharacter* OwnerCharacter = OwnerWeapon->GetWeaponOwnerCharacter();
	IWeaponCarrierInterface* WeaponCarrier = Cast<IWeaponCarrierInterface>(OwnerCharacter);
	if (WeaponCarrier)
	{
		WeaponTarget = WeaponCarrier->GetWeaponTargetActor();

		const float Distance = WeaponTarget ? (WeaponTarget->GetActorLocation() - OwnerCharacter->GetActorLocation()).Size() : 0.f;
		// Close to target if necessary
		if (Distance > ActionDistance)
		{
			WeaponCarrier->GetCarrierReachActionDistanceEvent().AddDynamic(this, &UWeaponActionBase::OnReachedActionDistance);
			WeaponCarrier->SetActionDistance(ActionDistance);
			WeaponCarrier->MoveToActionDistance();
		}
		else
		{
			OnReachedActionDistance(nullptr);
		}
	}
	else
	{
		UE_LOG(LogDogFight, Warning, TEXT("[WeaponAction] No valid target actor specified."));

		OnReachedActionDistance(nullptr);
	}
}

FName UWeaponActionBase::GetNextActionWithInput(EWeaponActionInput NewInput)
{
	for (FWeaponActionTransition Transition : ActionTransitions)
	{
		if (Transition.TransitionInput == NewInput)
		{
			return Transition.TargetActionName;
		}
	}

	return NAME_None;
}


void UWeaponActionBase::SetOwnerWeapon(UWeaponBase* NewWeapon)
{
	if (NewWeapon == nullptr)
		return;

	OwnerWeapon = NewWeapon;
}

FWeaponActionDisplayInfo UWeaponActionBase::GetWeaponActionDisplayInfo() const
{
	FWeaponActionDisplayInfo NewDisplayInfo;
	NewDisplayInfo.ActionName = WeaponActionName.ToString();
	NewDisplayInfo.ActionDescription = WeaponActionDescription;
	NewDisplayInfo.ActionRangeString = FString::Printf(TEXT("%.0f"), ActionDistance);
	const TArray<float> AllDamage = GetAllActionDamage();
	if (AllDamage.Num() > 0)
	{
		FString DamageString;
		for (float Damage : AllDamage)
		{
			if (!DamageString.IsEmpty())
			{
				DamageString.Append(TEXT(" + "));
			}

			DamageString.Append(FString::Printf(TEXT("%.2f"), Damage));
		}
		NewDisplayInfo.ActionDamageString = DamageString;
	}
	else
	{
		NewDisplayInfo.ActionDamageString = TEXT("0");
	}

	return NewDisplayInfo;
}

void UWeaponActionBase::PrepareActionMontage()
{
	PlayActionMontage(ActionMontage);
}

void UWeaponActionBase::PlayActionMontage(UAnimMontage* MontageToPlay)
{
	if (IsValid(MontageToPlay))
	{
		ACharacter* OwnerCharacter = OwnerWeapon->GetWeaponOwnerCharacter();
		AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(OwnerCharacter);
		if (StandardModePlayerCharacter != nullptr)
		{
			// Register callback to notifies
			for (FAnimNotifyEvent Notify : MontageToPlay->Notifies)
			{
				if (UAnimNotify_SwitchHitDetect* ApplyDamageNotify = Cast<UAnimNotify_SwitchHitDetect>(Notify.Notify))
				{
					ApplyDamageNotify->OnHitDetectSwitched.AddDynamic(this, &UWeaponActionBase::OnHitDetectSwitched);
				}
				else if (UAnimNotify_InvincibleFrame* InvincibleFrameNotify = Cast<UAnimNotify_InvincibleFrame>(Notify.Notify))
				{
					InvincibleFrameNotify->OnInvincibleStateChanged.AddDynamic(this, &UWeaponActionBase::OnInvincibleFrameChanged);
				}
			}
			
			const float ActionDuration = MontageToPlay->SequenceLength;
			StandardModePlayerCharacter->PlayMontage(MontageToPlay);
			GetWorld()->GetTimerManager().SetTimer(ActionTimerHandle, this, &UWeaponActionBase::OnActionMontageFinished, ActionDuration);
		}
	}
	else
	{
		// If no valid montage, trigger callback directly
		OnActionMontageFinished();
	}
}

TArray<float> UWeaponActionBase::GetAllActionDamage() const
{
	TArray<float> Result;

	if (IsValid(ActionMontage))
	{
		for (FAnimNotifyEvent Notify : ActionMontage->Notifies)
		{
			if (UAnimNotify_SwitchHitDetect* ApplyDamageNotify = Cast<UAnimNotify_SwitchHitDetect>(Notify.Notify))
			{
				// Only consider notify which are able to damage target
				if (ApplyDamageNotify->bTurnOn && !ApplyDamageNotify->CollisionParentSocketName.IsNone())
				{
					// Calculate action damage
					const AWeaponMeshActor* WeaponMeshActor = OwnerWeapon->GetWeaponMeshByParentSocket(ApplyDamageNotify->CollisionParentSocketName);
					const float WeaponBaseDamage = WeaponMeshActor ? WeaponMeshActor->GetBaseDamage() : 0.f;

					const float FinalDamage = ApplyDamageNotify->DamageRatio * WeaponBaseDamage;
					if (FinalDamage > 0.f)
					{
						Result.Add(FinalDamage);
					}
				}
			}
		}
	}

	return Result;
}

void UWeaponActionBase::OnReachedActionDistance(AActor* Carrier)
{
	AActor* TargetActor = nullptr;
	// Unregister callback
	if (IsValid(Carrier))
	{
		if (IWeaponCarrierInterface* WeaponCarrier = Cast<IWeaponCarrierInterface>(Carrier))
		{
			WeaponCarrier->GetCarrierReachActionDistanceEvent().RemoveDynamic(this, &UWeaponActionBase::OnReachedActionDistance);
			TargetActor = WeaponCarrier->GetWeaponTargetActor();
		}
	}

	// Request response card from target player
	bool bWaitingResponseCard = false;
	ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);
	if (IsValid(TargetCharacter))
	{
		APlayerState* PlayerState = TargetCharacter->GetPlayerState();
		if (ResponseCardClasses.Num() > 0 && IsValid(PlayerState))
		{
			// TODO: Remove this hack debug condition
			//if (PlayerState->GetPlayerId() >= 100)
			{
				if (AGameModeBase* GameModeBase = GetWorld()->GetAuthGameMode())
				{
					if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GameModeBase))
					{
						bWaitingResponseCard = true;
						StandardGameMode->OnPlayerResponseCardSelected.AddDynamic(this, &UWeaponActionBase::OnResponseCardSelected);
						StandardGameMode->RequestResponseCardFromPlayer(PlayerState->GetPlayerId(), ResponseCardClasses, Carrier, FText::FromName(WeaponActionName));
					}
				}
			}
		}
	}

	// Play montage directly if no need to wait response card
	if (!bWaitingResponseCard)
	{
		PrepareActionMontage();
	}
}

void UWeaponActionBase::OnActionMontageFinished()
{
	if (ActionTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ActionTimerHandle);
	}

	// Trigger callback
	OnWeaponActionFinished.Broadcast();
}

void UWeaponActionBase::OnResponseCardSelected()
{
	// Unregister callback
	if (AGameModeBase* GameModeBase = GetWorld()->GetAuthGameMode())
	{
		if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GameModeBase))
		{
			StandardGameMode->OnPlayerResponseCardSelected.RemoveDynamic(this, &UWeaponActionBase::OnResponseCardSelected);
		}
	}

	PrepareActionMontage();
}

void UWeaponActionBase::OnHitDetectSwitched(UAnimNotify_SwitchHitDetect* Notify, bool bTurnOn, float DamageRatio, FName ParentSocketName)
{
	// Unregister callback
	if (Notify)
	{
		Notify->OnHitDetectSwitched.RemoveDynamic(this, &UWeaponActionBase::OnHitDetectSwitched);
	}

	if (ParentSocketName.IsNone())
	{
		UE_LOG(LogDogFight, Warning, TEXT("[WeaponActionBase] Invalid bone name specified."));
		return;
	}

	// Get collision test actor
	if (AWeaponMeshActor* MeshActor = OwnerWeapon->GetWeaponMeshByParentSocket(ParentSocketName))
	{
		MeshActor->SetDamageRatio(DamageRatio);
		MeshActor->SetDetectHit(bTurnOn);
	}
}

void UWeaponActionBase::OnInvincibleFrameChanged(UAnimNotify_InvincibleFrame* Notify, bool bInvincible)
{
	// Unregister callback after invincible disable
	if (Notify && !bInvincible)
	{
		Notify->OnInvincibleStateChanged.RemoveDynamic(this, &UWeaponActionBase::OnInvincibleFrameChanged);
	}

	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(OwnerWeapon->GetWeaponOwnerCharacter()))
	{
		if (bInvincible)
		{
			StandardModePlayerCharacter->AddInvincibleFlags(static_cast<int32>(EActorInvincibleFlags::AIF_InvincibleFrame));
		}
		else
		{
			StandardModePlayerCharacter->RemoveInvincibleFlags(static_cast<int32>(EActorInvincibleFlags::AIF_InvincibleFrame));
		}
	}
}

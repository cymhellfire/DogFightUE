// Dog Fight Game Code By CYM.


#include "Actors/Weapons/WeaponActionBase.h"

#include "Actors/Interfaces/WeaponCarrierInterface.h"
#include "Actors/Weapons/WeaponBase.h"
#include "Game/StandardGameMode.h"

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

void UWeaponActionBase::PrepareActionMontage()
{
	PlayActionMontage(ActionMontage);
}

void UWeaponActionBase::PlayActionMontage(UAnimMontage* MontageToPlay)
{
	if (IsValid(MontageToPlay))
	{
		ACharacter* OwnerCharacter = OwnerWeapon->GetWeaponOwnerCharacter();
		USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
		if (MeshComponent != nullptr)
		{
			if (UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance())
			{
				const float ActionDuration = AnimInstance->Montage_Play(MontageToPlay);
				GetWorld()->GetTimerManager().SetTimer(ActionTimerHandle, this, &UWeaponActionBase::OnActionMontageFinished, ActionDuration);
			}
		}
	}
	else
	{
		// If no valid montage, trigger callback directly
		OnActionMontageFinished();
	}
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
			if (PlayerState->GetPlayerId() >= 100)
			{
				if (AGameModeBase* GameModeBase = GetWorld()->GetAuthGameMode())
				{
					if (AStandardGameMode* StandardGameMode = Cast<AStandardGameMode>(GameModeBase))
					{
						bWaitingResponseCard = true;
						StandardGameMode->OnPlayerResponseCardSelected.AddDynamic(this, &UWeaponActionBase::OnResponseCardSelected);
						StandardGameMode->RequestResponseCardFromPlayer(PlayerState->GetPlayerId(), ResponseCardClasses, Carrier);
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

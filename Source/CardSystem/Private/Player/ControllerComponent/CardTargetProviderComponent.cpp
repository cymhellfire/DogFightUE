﻿#include "Player/ControllerComponent/CardTargetProviderComponent.h"

#include "CardSystem.h"

UCardTargetProviderComponent::UCardTargetProviderComponent()
{
	bTargetSelecting = false;
	bWantsInitializeComponent = true;
}

void UCardTargetProviderComponent::AcquireTarget(FTargetAcquireSettings InSettings)
{
	TargetCount = InSettings.TargetCount;
	CurrentTargetType = InSettings.Type;

	ClearAllTargetInfo();
	bTargetSelecting = true;
}

void UCardTargetProviderComponent::InitializeInput()
{
	PlayerController = GetTypedOuter<APlayerController>();
	if (PlayerController.IsValid())
	{
		PlayerController->InputComponent->BindAction("ConfirmSelection", IE_Pressed, this, &UCardTargetProviderComponent::OnSelectTargetPressed);
		PlayerController->InputComponent->BindAction("CancelSelection", IE_Pressed, this, &UCardTargetProviderComponent::OnCancelSelectionPressed);
	}
	else
	{
		UE_LOG(LogCardSystem, Error, TEXT("[CardTargetProviderComponent] Expect an outer with APlayerController type, some functionalities are unavailable."));
	}
}

void UCardTargetProviderComponent::OnSelectTargetPressed()
{
	// Ignore when selecting is not enabled
	if (!bTargetSelecting)
	{
		return;
	}

	// Get hit result
	FHitResult HitResult;
	ECollisionChannel HitChannel = CurrentTargetType == ECardTargetType::CTT_Actor ? ECC_GameTraceChannel1 : ECC_Visibility;
	PlayerController->GetHitResultUnderCursor(HitChannel, false, HitResult);
	if (HitResult.bBlockingHit)
	{
		FAcquiredTargetInfo NewTargetInfo;
		NewTargetInfo.Type = CurrentTargetType;
		switch(CurrentTargetType)
		{
		case ECardTargetType::CTT_Point:
			NewTargetInfo.Vector = HitResult.Location;
			break;
		case ECardTargetType::CTT_Direction:
			NewTargetInfo.Vector = HitResult.Location - PlayerController->GetPawn()->GetActorLocation();
			// Erase the value on Z axis
			NewTargetInfo.Vector.Z = 0;
			break;
		case ECardTargetType::CTT_Actor:
			NewTargetInfo.ActorPtr = HitResult.GetActor();
			break;
		default:
			UE_LOG(LogCardSystem, Error, TEXT("[CardTargetProviderComponent] Using invalid target type."));
			break;
		}

		// Record new target
		CachedTargetInfo.Add(NewTargetInfo);
		if (CachedTargetInfo.Num() >= TargetCount)
		{
			OnAllTargetAcquired();
		}
	}
}

void UCardTargetProviderComponent::OnAllTargetAcquired()
{
	// Disable selecting
	bTargetSelecting = false;

	OnCardTargetAcquired.Broadcast(true);
}

void UCardTargetProviderComponent::OnCancelSelectionPressed()
{
	if (bTargetSelecting)
	{
		bTargetSelecting = false;

		OnCardTargetAcquired.Broadcast(false);
	}
}

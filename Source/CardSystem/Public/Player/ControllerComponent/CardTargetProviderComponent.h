#pragma once

#include "CoreMinimal.h"
#include "Player/CardTargetProviderInterface.h"
#include "CardTargetProviderComponent.generated.h"

/**
 * Logic component that provides card target selecting functionalities.
 */
UCLASS(BlueprintType)
class CARDSYSTEM_API UCardTargetProviderComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UCardTargetProviderComponent();

	void AcquireTarget(FTargetAcquireSettings InSettings);

	TArray<FAcquiredTargetInfo> GetLastTargetInfoList() const
	{
		return CachedTargetInfo;
	}

	void ClearAllTargetInfo()
	{
		CachedTargetInfo.Empty();
	}

	void InitializeInput();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCardTargetAcquireFinished, bool, bSuccessed);
	FCardTargetAcquireFinished OnCardTargetAcquired;

protected:

	void OnSelectTargetPressed();
	void OnCancelSelectionPressed();

	void OnAllTargetAcquired();

	uint8 bTargetSelecting : 1;

	int32 TargetCount;
	ECardTargetType CurrentTargetType;

	TArray<FAcquiredTargetInfo> CachedTargetInfo;

	TWeakObjectPtr<APlayerController> PlayerController;
};

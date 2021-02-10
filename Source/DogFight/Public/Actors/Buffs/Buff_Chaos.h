// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Buffs/BuffBase.h"
#include "Buff_Chaos.generated.h"

/**
 * A buff that has chance to let player hit self.
 */
UCLASS()
class DOGFIGHT_API ABuff_Chaos : public ABuffBase
{
	GENERATED_BODY()

protected:
	virtual void ApplyBuff() override;
	virtual void RemoveBuff() override;

	void OnTargetActorSelected(AActor** SelectedActor);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff", meta=(ClampMin="0", ClampMax="1"))
	float ChaosChance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff")
	FLocalizedString ChaosTriggerText;

protected:
	FDelegateHandle OnTargetActorSelectedHandle;
};

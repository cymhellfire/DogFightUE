// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "StandardGameModePhaseBase.h"
#include "StandardGameModeDecideOrderPhase.generated.h"

/**
 * Game phase that initialize player action order and initialize all players for game.
 */
UCLASS(Blueprintable)
class DOGFIGHT_API UStandardGameModeDecideOrderPhase : public UStandardGameModePhaseBase
{
	GENERATED_BODY()
public:
	UStandardGameModeDecideOrderPhase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool StartPhase() override;

	UFUNCTION()
	void OnWeaponEquipped(AActor* CarrierActor);

private:
	typedef UStandardGameModePhaseBase Super;

	int32 WeaponEquipWaitingCharacterCount;
};

// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "Game/GameWorkflow/GamePhase.h"
#include "StandardGameModePhaseBase.generated.h"

class AStandardGameMode;

/**
 * Base class of all game phases used in Standard Game Mode.
 */
UCLASS()
class DOGFIGHT_API UStandardGameModePhaseBase : public UGamePhase
{
	GENERATED_BODY()
public:
	UStandardGameModePhaseBase(const FObjectInitializer& ObjectInitializer);

	virtual void SetOwnerGameMode(AGameModeBase* InGameMode) override;

protected:
	virtual bool StartPhase() override;
	virtual void EndPhase() override;
	virtual void InterruptPhase() override;
	virtual void ResumePhase() override;

protected:
	AStandardGameMode* ParentStandardGameMode;
};

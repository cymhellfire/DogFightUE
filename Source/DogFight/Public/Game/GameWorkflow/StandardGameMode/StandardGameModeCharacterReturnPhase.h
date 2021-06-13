// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "StandardGameModePhaseBase.h"
#include "StandardGameModeCharacterReturnPhase.generated.h"

UCLASS()
class UStandardGameModeCharacterReturnPhase : public UStandardGameModePhaseBase
{
	GENERATED_BODY()
protected:
	virtual bool StartPhase() override;

	UFUNCTION()
	void OnCharacterReturnFinished(AActor* Actor);

protected:
	int32 ReturnedCharacterCount;
};

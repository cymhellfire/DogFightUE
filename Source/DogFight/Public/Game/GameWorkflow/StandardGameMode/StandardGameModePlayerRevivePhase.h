// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "StandardGameModePhaseBase.h"
#include "StandardGameModePlayerRevivePhase.generated.h"

class AStandardModePlayerCharacter;

UCLASS()
class UStandardGameModePlayerRevivePhase : public UStandardGameModePhaseBase
{
	GENERATED_BODY()
public:
	UStandardGameModePlayerRevivePhase(const FObjectInitializer& ObjectInitializer);

	void RegisterListenCharacter(AStandardModePlayerCharacter* NewCharacter);

	void ClearListeningCharacters();

	virtual void BeginDestroy() override;
protected:
	virtual void ResumePhase() override;

	UFUNCTION()
	void OnCharacterReviveStateChanged(AStandardModePlayerCharacter* NewCharacter, bool bActive);

protected:
	int32 RevivingCharacters;

	TArray<AStandardModePlayerCharacter*> ListeningCharacters;
};

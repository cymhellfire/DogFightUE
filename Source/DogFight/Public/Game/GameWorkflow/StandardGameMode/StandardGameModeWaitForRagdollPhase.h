// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "StandardGameModePhaseBase.h"
#include "StandardGameModeWaitForRagdollPhase.generated.h"

class AStandardModePlayerCharacter;

/**
 * Game phase that waiting for all characters in ragdoll state.
 */
UCLASS()
class UStandardGameModeWaitForRagdollPhase : public UStandardGameModePhaseBase
{
	GENERATED_BODY()
public:
	UStandardGameModeWaitForRagdollPhase(const FObjectInitializer& ObjectInitializer);

	void RegisterListenCharacter(AStandardModePlayerCharacter* NewCharacter);

	void ClearListeningCharacters();

	virtual void BeginDestroy() override;
protected:
	UFUNCTION()
	void OnCharacterRagdollStateChanged(AStandardModePlayerCharacter* Character, bool bActive);

protected:
	int32 RagdollCount;

	TArray<AStandardModePlayerCharacter*> ListeningCharacters;
};

#pragma once

#include "GameEffectBase.h"
#include "GameEffectSequence.generated.h"

class ULevelSequence;
class ALevelSequenceActor;

/**
 * Effect constructed by level sequence. 
 */
UCLASS(Blueprintable)
class DOGFIGHTGAME_API AGameEffectSequence : public AGameEffectBase
{
	GENERATED_BODY()

public:
	AGameEffectSequence(const FObjectInitializer& ObjectInitializer);

	virtual void PlayEffect() override;

protected:
	ALevelSequenceActor* GetOrCreateSequenceActor();

	UFUNCTION()
	void OnSequenceFinished();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameEffect")
	TSoftObjectPtr<ULevelSequence> LevelSequence;

protected:
	uint8 bPlaying : 1;

	TWeakObjectPtr<ALevelSequenceActor> OwnedSequenceActor;
};

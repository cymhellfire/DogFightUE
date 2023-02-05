#pragma once

#include "CoreMinimal.h"
#include "GameEffectBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FGameEffectFinishDelegate, AGameEffectBase*)

/**
 * Base class of all in-game effects.
 */
UCLASS(Abstract, NotBlueprintable)
class DOGFIGHTGAME_API AGameEffectBase : public AActor
{
	GENERATED_BODY()

public:
	AGameEffectBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(CallInEditor, Category="GameEffect")
	virtual void PlayEffect() {}

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GameEffect")
	int32 EffectId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameEffect")
	FString EffectName;

	FGameEffectFinishDelegate OnEffectFinished;
};

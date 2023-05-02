#pragma once

#include "CoreMinimal.h"
#include "GameEffectBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameEffectFinishDelegate, AGameEffectBase*, Effect);

/**
 * Base class of all in-game effects.
 */
UCLASS(Abstract, NotBlueprintable)
class DOGFIGHTGAME_API AGameEffectBase : public AActor
{
	GENERATED_BODY()

public:
	AGameEffectBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(CallInEditor, Category="GameEffect", NetMulticast, Reliable)
	virtual void PlayEffect();

	/**
	 * Attach this effect to given actor.
	 * @param InTarget		Target actor this effect attach to.
	 */
	UFUNCTION(BlueprintCallable, Category="GameEffect")
	virtual void SetTarget(AActor* InTarget) {}

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GameEffect")
	int32 EffectId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameEffect")
	FString EffectName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameEffect")
	FName AttachSocketName;

	UPROPERTY(BlueprintAssignable, Category="GameEffect")
	FGameEffectFinishDelegate OnEffectFinished;
};

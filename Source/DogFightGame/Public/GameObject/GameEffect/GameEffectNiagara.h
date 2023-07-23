#pragma once

#include "GameEffectBase.h"
#include "GameEffectNiagara.generated.h"

class UNiagaraComponent;

/**
 * Effect constructed by niagara system.
 */
UCLASS(Blueprintable)
class DOGFIGHTGAME_API AGameEffectNiagara : public AGameEffectBase
{
	GENERATED_BODY()

public:
	AGameEffectNiagara(const FObjectInitializer& ObjectInitializer);

	virtual void SetTarget(AActor* InTarget) override;

	virtual void PlayEffect_Implementation() override;

protected:
	virtual void BeginPlay() override;

	void CheckForFinished();

	UFUNCTION()
	void OnNiagaraSystemFinished(UNiagaraComponent* InComponent);

	UFUNCTION()
	void OnAudioFinished();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameEffect")
	bool bAutoPlay;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GameEffect")
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GameEffect")
	UAudioComponent* AudioComponent;

	uint8 bNiagaraOn : 1;
	uint8 bAudioOn : 1;
};

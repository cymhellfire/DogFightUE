// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_FacingTarget.generated.h"

/**
 * AnimNotifyState used to define a window to adjusting character facing direction.
 */
UCLASS(meta = (DisplayName = "Facing Target"))
class ACTIONGAME_API UAnimNotifyState_FacingTarget : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_FacingTarget();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

private:
	void SetupNotifyData(AActor* OwnerActor, UAnimMontage* InMontage);

public:
	UPROPERTY(EditAnywhere, Category="FacingTarget")
	FString TargetName;

private:
	float Process;
	float Duration;

	FRotator InitialRotator;
	FRotator FinalRotator;
};

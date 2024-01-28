// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_AttackDetect.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FAnimNotifyStateAttackDetechEvent, bool);

/**
 * AnimNotifyState used to define a window for attack detecting.
 */
UCLASS(meta = (DisplayName = "Attack Detect"))
class ACTIONGAME_API UAnimNotifyState_AttackDetect : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_AttackDetect();
	virtual void BeginDestroy() override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	FAnimNotifyStateAttackDetechEvent OnAttackDetectStateChanged;

private:
	bool bAttackDetectOn;
};

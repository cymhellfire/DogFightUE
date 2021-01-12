// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/CardInstructionBase.h"
#include "Animation/AnimNotifyDelegateAction.h"
#include "InstructionPlayAnimation.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMIZABLECARD_API UInstructionPlayAnimation : public UCardInstructionBase
{
	GENERATED_BODY()

public:

	UInstructionPlayAnimation(const FObjectInitializer& ObjectInitializer);

	virtual void Execute() override;

	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Category="InstructionPlayAnimation")
	void AddAnimNotifyDelegateAction(const FAnimNotifyDelegateAction& NewAction);

protected:

	void BindAllDelegateActions();

	UFUNCTION()
	void OnAnimNotifyDelegateTriggered(UAnimNotify_Delegate* NotifyInstance);

	UFUNCTION()
	void OnAnimationFinished();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CardInstruction")
	UAnimMontage* MontageToPlay;

	FTimerHandle AnimationWaitingTimerHandle;

	TArray<FAnimNotifyDelegateAction> DelegateActions;

	TMap<UAnimNotify_Delegate*, TArray<FAnimNotifyDelegateAction>> ActionMap;
};

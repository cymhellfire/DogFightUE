// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "CardInstructionBase.h"
#include "HandleTargetInstructionBase.generated.h"

struct FCardInstructionTargetInfo;

/**
 * Base class of all instructions are able to handle target information.
 */
UCLASS(Abstract)
class DOGFIGHT_API UHandleTargetInstructionBase : public UCardInstructionBase
{
	GENERATED_BODY()

public:
	UHandleTargetInstructionBase(const FObjectInitializer& ObjectInitializer);

	/**
	 * Process the target information store in owner card.
	 * @return True if all targets are processed.
	 */
	virtual void ProcessTarget();

	virtual void Execute() override;

	virtual void Finish() override;
protected:

	/**
	 * Parse the passed in target information.
	 * @param TargetInfo		The target information struct to parse
	 * @param ActorPtr			The parsed Actor pointer (non-null when return 1)
	 * @param PositionValue		The parsed Position value
	 * @param DirectionValue	The parsed direction value
	 * @return Number represent the target type: 0.Invalid 1.Actor 2.Position 3.Direction
	 */
	int32 ParseTargetInfo(FCardInstructionTargetInfo TargetInfo, AActor** ActorPtr, FVector& PositionValue, FVector& DirectionValue);

	/**
	 * Handle a Actor type target.
	 * @param Target			The target actor.
	 * @return Whether the given target is handled successfully
	 */
	virtual bool HandleActorTarget(AActor* Target);

	virtual void HandlePositionTarget(FVector Position);

	virtual void HandleDirectionTarget(FVector Direction);

	virtual void OnHandledAllTarget() {};

	void OnHandleTargetIntervalTimerExpired();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	float HandleTargetInterval;

	/** Only send one message when this option is on no matter how many targets. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	bool bMergeTargetMessage;

	int32 CurrentTargetIndex;

	/** Does this instruction invoke Finish() automatically at the end of Execute()? (True as default)*/
	bool bAutoFinish;

	/** Skip next broadcast game message. (Use this to suppress redundant message once.) */
	bool bSkipOneBroadcast;

	bool bUseMessageSent;

	FTimerHandle HandleTargetIntervalTimerHandle;
};

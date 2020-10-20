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
class CUSTOMIZABLECARD_API UHandleTargetInstructionBase : public UCardInstructionBase
{
	GENERATED_BODY()

public:
	UHandleTargetInstructionBase(const FObjectInitializer& ObjectInitializer);

	/**
	 * Process the target information store in owner card.
	 * @return True if all targets are processed.
	 */
	virtual bool ProcessTarget();

	virtual void Execute() override;
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

	virtual void HandleActorTarget(AActor* Target) {};

	virtual void HandlePositionTarget(FVector Position) {};

	virtual void HandleDirectionTarget(FVector Direction) {};

	int32 CurrentTargetIndex;

	/** Does this instruction invoke Finish() automatically at the end of Execute()? (True as default)*/
	bool bAutoFinish;
};

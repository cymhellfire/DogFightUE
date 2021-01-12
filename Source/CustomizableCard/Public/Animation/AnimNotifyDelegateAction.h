// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify_Delegate.h"
#include "Gameplay/CardInstructionBase.h"
#include "AnimNotifyDelegateAction.generated.h"

/**
 * Struct to hold information for executing card instruction when AnimNotify_Delegate
 * is triggered.
 */
USTRUCT(BlueprintType)
struct FAnimNotifyDelegateAction
{
	GENERATED_BODY();

	/** Target delegate type bind to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotifyDelegateAction")
	EAnimNotifyDelegateType TargetDelegateType;

	/** The desire index to bind if there are multiple delegate match specified type. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotifyDelegateAction")
	int32 DesireIndex;

	/** Card instruction to execute when delegate is invoke. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotifyDelegateAction")
	UCardInstructionBase* InstructionToExecute;
};

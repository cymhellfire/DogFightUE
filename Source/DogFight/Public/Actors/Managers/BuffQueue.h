// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuffQueue.generated.h"

class ABuffBase;

/**
 * Queue of buff on owner actor.
 */
UCLASS()
class DOGFIGHT_API UBuffQueue : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBuffQueueProcessFinishedSignature);
	/** Trigger when all buff in queue is processed. */
	FBuffQueueProcessFinishedSignature OnBuffQueueProcessFinished;

	/** Register a buff to queue. */
	void RegisterBuff(ABuffBase* NewBuff);

	/** Remove a buff from queue. */
	void UnregisterBuff(ABuffBase* TargetBuff);

	/** Start process to handle buff list and end buffs. */
	void StartBuffCheckProcess();

	int32 GetBuffCount() const { return AttachedBuffList.Num(); }

protected:

	void ProcessCurrentBuff();

	UFUNCTION()
	void OnBuffEnded(ABuffBase* Buff);

protected:
	/** All buff instance attached to this actor. */
	TArray<ABuffBase*> AttachedBuffList;

	int32 CurrentBuffIndex;
};

// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuffQueue.generated.h"

class ABuffBase;

enum EQueueProcessPhase
{
	EQP_RoundBegin,
	EQP_RoundEnd,
};

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

	/** Start process to handle buff list and end buffs. (At owning player round begin) */
	void StartRoundBeginBuffCheckProcess();

	/** Start process to handle buff list and end buffs. (At owning player round end) */
	void StartRoundEndBuffCheckProcess();

	int32 GetBuffCount() const { return AttachedBuffList.Num(); }

	/** Get total count of buff with given class. */
	int32 GetBuffCountOfType(FName BuffClassName) const;

protected:

	void ProcessCurrentBuff();

	UFUNCTION()
	void OnBuffEnded(ABuffBase* Buff);

	UFUNCTION()
	void OnBuffProcessEnded(ABuffBase* Buff);

protected:
	/** All buff instance attached to this actor. */
	TArray<ABuffBase*> AttachedBuffList;

	/** Map records count of different types of buff. */
	TMap<FName, int32> BuffCountMap;

	int32 CurrentBuffIndex;

	EQueueProcessPhase CurrentProcessPhase;
};

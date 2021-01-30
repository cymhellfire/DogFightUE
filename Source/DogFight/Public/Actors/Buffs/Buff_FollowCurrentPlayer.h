// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Buffs/BuffBase.h"
#include "Buff_FollowCurrentPlayer.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ABuff_FollowCurrentPlayer : public ABuffBase
{
	GENERATED_BODY()

protected:
	virtual void OnTargetPlayerRoundEnd() override;

	virtual void MoveToNextPlayer();

	virtual void OnDetachFromCurrentPlayer(int32 PlayerId);

	virtual void OnAttachToNextPlayer(int32 PlayerId);
};

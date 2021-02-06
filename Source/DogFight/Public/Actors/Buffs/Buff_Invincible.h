// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Buffs/Buff_Shield.h"
#include "Buff_Invincible.generated.h"

/**
 * A buff that can make target actor invincible during lifetime.
 */
UCLASS()
class DOGFIGHT_API ABuff_Invincible : public ABuff_Shield
{
	GENERATED_BODY()

protected:
	virtual void ApplyBuff() override;
	virtual void RemoveBuff() override;
};

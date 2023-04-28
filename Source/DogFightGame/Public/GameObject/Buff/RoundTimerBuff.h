// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "NewBuffBase.h"
#include "Interface/RoundBasedLifetimeInterface/RoundBasedLifetimeInterface.h"
#include "RoundTimerBuff.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API URoundTimerBuff : public UNewBuffBase, public IRoundBasedLifetimeInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="RoundTimerBuff")
	void SetDuration(int32 InTime);

protected:
	virtual UObject* GetWorldContext() override
	{
		return this;
	}
};

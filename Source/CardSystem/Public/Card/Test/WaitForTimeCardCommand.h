#pragma once

#include "Card/CardAsyncCommand.h"
#include "WaitForTimeCardCommand.generated.h"

USTRUCT(BlueprintType)
struct FWaitForTimeCardCommandParam
{
	GENERATED_BODY()

	UPROPERTY()
	float Time;
};

UCLASS()
class CARDSYSTEM_API UWaitForTimeCardCommand : public UCardAsyncCommand
{
	GENERATED_BODY()
public:

	virtual bool Initialize(void* Param) override;

	virtual void Run() override;

protected:
	UFUNCTION()
	void OnTimerFinished();

protected:
	float WaitTime;

	FTimerHandle TimerHandle;
};

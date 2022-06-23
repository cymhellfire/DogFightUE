#pragma once

#include "Card/CardConcurrentCallbackCommand.h"
#include "WaitAndRandomCallbackCardCommand.generated.h"

USTRUCT(BlueprintType)
struct FWaitAndRandomCallbackCardCommandParam
{
	GENERATED_BODY()

	UPROPERTY()
	float Time;

	UPROPERTY()
	FVector2D RandomRange;
};

UCLASS()
class UWaitAndRandomCallbackCardCommand : public UCardConcurrentCallbackCommand
{
	GENERATED_BODY()
public:

	virtual bool Initialize(void* Param) override;

	virtual void Run() override;

protected:
	UFUNCTION()
	void OnTimerExpired();

protected:
	float Time;
	FVector2D Range;
	FTimerHandle TimerHandle;
};

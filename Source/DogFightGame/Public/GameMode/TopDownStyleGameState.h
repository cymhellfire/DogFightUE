#pragma once

#include "GameMode/DogFightGameStateBase.h"
#include "TopDownStyleGameState.generated.h"

class UGameTimelineComponent;

UCLASS()
class DOGFIGHTGAME_API ATopDownStyleGameState : public ADogFightGameStateBase
{
	GENERATED_BODY()
public:
	ATopDownStyleGameState(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	UGameTimelineComponent* GetGameTimelineComponent() const
	{
		return GameTimelineComponent;
	}

protected:
	UFUNCTION()
	void OnTimelineChanged();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TopDownGame")
	FBox WorldBounds;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TopDownGameState")
	UGameTimelineComponent* GameTimelineComponent;
};

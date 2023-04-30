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

	void SetCurrentPlayerId(int32 InId);

	int32 GetCurrentPlayerId() const
	{
		return CurrentPlayerId;
	}

	APlayerState* GetPlayerStateById(int32 InPlayerId) const;

	UGameTimelineComponent* GetGameTimelineComponent() const
	{
		return GameTimelineComponent;
	}

protected:
	void OnTimelineChanged();

	UFUNCTION()
	void OnRep_CurrentPlayerId();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TopDownGame")
	FBox WorldBounds;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TopDownGameState")
	UGameTimelineComponent* GameTimelineComponent;

	UPROPERTY(Transient, ReplicatedUsing=OnRep_CurrentPlayerId)
	int32 CurrentPlayerId;
};

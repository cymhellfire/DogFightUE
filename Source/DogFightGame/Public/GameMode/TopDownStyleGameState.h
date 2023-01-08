#pragma once

#include "GameMode/DogFightGameStateBase.h"
#include "TopDownStyleGameState.generated.h"

UCLASS()
class DOGFIGHTGAME_API ATopDownStyleGameState : public ADogFightGameStateBase
{
	GENERATED_BODY()
public:
	ATopDownStyleGameState(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TopDownGame")
	FBox WorldBounds;
};

// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BotPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHTAI_API ABotPlayerController : public AAIController
{
	GENERATED_BODY()
public:
	ABotPlayerController();

	virtual void InitPlayerState() override;

	virtual void SetPlayerId(int32 InId);
};

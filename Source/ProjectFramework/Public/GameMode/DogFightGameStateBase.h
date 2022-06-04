// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DogFightGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTFRAMEWORK_API ADogFightGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	void FinishGameAndReturnToMainMenu();
};

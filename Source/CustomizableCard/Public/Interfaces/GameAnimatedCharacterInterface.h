// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameAnimatedCharacterInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UGameAnimatedCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface that allows instruction play montage.
 */
class IGameAnimatedCharacterInterface
{
	GENERATED_BODY()

public:

	/** Let character play montage. */
	virtual float PlayMontage(UAnimMontage* MontageToPlay) { return 0.f; };
};

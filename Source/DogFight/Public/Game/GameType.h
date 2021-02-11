#pragma once

#include "CoreMinimal.h"
#include "GameType.generated.h"

/**
 * Game phases that can be skipped.
 */
UENUM(BlueprintType, meta=(Bitflags))
enum ESkippableGamePhase
{
	ESGP_GiveCards		UMETA(DisplayName="Give Cards"),
	ESGP_UseCards		UMETA(DisplayName="Use Cards"),
	ESGP_DropCards		UMETA(DisplayName="Drop Cards"),
};

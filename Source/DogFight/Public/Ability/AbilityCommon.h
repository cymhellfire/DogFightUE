// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"

#include "AbilityCommon.generated.h"

UENUM(BlueprintType)
enum class EAbilityCastType : uint8
{
	ACT_Aggressive		UMETA(DisplayName="Aggressive"),
	ACT_Passive			UMETA(DisplayName="Passive")
};

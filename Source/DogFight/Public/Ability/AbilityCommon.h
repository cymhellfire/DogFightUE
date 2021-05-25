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

UENUM(meta=(Bitflags))
enum class EAbilityAvailablePhase
{
	AAP_PrePlayerRoundBegin		UMETA(DisplayName="Before Player Round"),
	AAP_DuringPlayerRound		UMETA(DisplayName="During Player Round"),
	AAP_DiscardCard				UMETA(DisplayName="Discard Card"),
	AAP_OwnerPhase				UMETA(DisplayName="Owner Phases"),
	AAP_OtherPhase				UMETA(DisplayName="Other Player Phases"),
};

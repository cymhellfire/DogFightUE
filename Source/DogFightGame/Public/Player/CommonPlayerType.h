#pragma once

#include "CommonPlayerType.generated.h"

/** Enum that presents different state of player. */
UENUM(BlueprintType)
namespace ETopDownStylePlayerState
{
	enum Type
	{
		PS_None			UMETA(DisplayName="None"),
		PS_Alive		UMETA(DisplayName="Alive"),
		PS_Dead			UMETA(DisplayName="Dead"),
	};
}

#pragma once

#include "InputMappingDef.generated.h"

UENUM(BlueprintType)
namespace EInputMappingType
{
	enum Type
	{
		InputMapping_CameraMove,
		InputMapping_CharacterMove,
		InputMapping_CardUsing,
		InputMapping_InGameCommon,
	};
}

UENUM(BlueprintType)
namespace EInputMappingPriority
{
	enum Type
	{
		Lowest = 0,
		Lower = 1,
		Normal = 2,
		Higher = 3,
		Highest = 4,
	};
}

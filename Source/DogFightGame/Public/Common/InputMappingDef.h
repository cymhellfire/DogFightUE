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
	};
}

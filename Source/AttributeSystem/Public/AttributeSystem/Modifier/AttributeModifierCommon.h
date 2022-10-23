#pragma once

#include "AttributeModifierCommon.generated.h"

UENUM()
enum class EModifierOperatorType
{
	MOT_Equal,
	MOT_Addition,
	MOT_Subtract,
	MOT_Multiply,
	MOT_Divide,
};

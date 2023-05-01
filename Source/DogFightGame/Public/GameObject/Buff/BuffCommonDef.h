#pragma once

#include "BuffCommonDef.generated.h"

UENUM(BlueprintType)
namespace EBuffCheckType
{
	enum Type
	{
		Default,
		PrePlayerRound,
		PostPlayerRound,
	};
}

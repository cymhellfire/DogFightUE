#pragma once

#include "CoreMinimal.h"
#include "ActionAnimCommon.generated.h"

UENUM(BlueprintType)
namespace EActionAnimPredefinedType
{
	enum Type
	{
		None,
		Attack1			UMETA(DisplayName="Attack 1"),
		Attack2			UMETA(DisplayName="Attack 2"),
		Defence1		UMETA(DisplayName="Defence 1"),
	};
}

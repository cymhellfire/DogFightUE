// Dog Fight Game Code By CYM.
#pragma once

#include "PropertyModifyCommon.generated.h"

UENUM(BlueprintType)
enum class EPropertyModifyMethod : uint8
{
	PMM_Add				UMETA(DisplayName="Add"),
	PMM_Subtract		UMETA(DisplayName="Subtract"),
	PMM_Multiply		UMETA(DisplayName="Multiply"),
	PMM_Replace			UMETA(DisplayName="Replace"),
};
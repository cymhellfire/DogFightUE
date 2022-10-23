#pragma once

#include "AttributeSystemCommon.generated.h"

UENUM(BlueprintType)
enum EAttributeDataType
{
	ADT_None		UMETA(DisplayName="None"),
	ADT_Boolean		UMETA(DisplayName="Boolean"),
	ADT_Integer		UMETA(DisplayName="Integer"),
	ADT_Float		UMETA(DisplayName="Float"),
};

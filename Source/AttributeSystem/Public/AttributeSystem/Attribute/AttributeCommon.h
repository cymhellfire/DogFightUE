#pragma once

#include "AttributeSystem/AttributeSystemCommon.h"
#include "AttributeCommon.generated.h"

USTRUCT(BlueprintType)
struct ATTRIBUTESYSTEM_API FAttributeCreateArgument
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttributeArgument")
	FName AttrName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttributeArgument")
	TEnumAsByte<EAttributeDataType> DataType;
};

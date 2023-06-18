#pragma once

#include "AttributeSystem/AttributeSystemCommon.h"
#include "AttributeCommon.generated.h"

UENUM(BlueprintType)
enum EAttributeFlag
{
	AF_None = 0,
	AF_AutoAdd = 1 << 0,
};

USTRUCT(BlueprintType)
struct ATTRIBUTESYSTEM_API FAttributeCreateArgument
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttributeArgument")
	FName AttrName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttributeArgument")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttributeArgument")
	TEnumAsByte<EAttributeDataType> DataType = ADT_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttributeArgument")
	TArray<FString> Tags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttributeArgument")
	bool InitBooleanValue = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttributeArgument")
	int32 InitIntegerValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttributeArgument")
	float InitFloatValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttributeArgument")
	int32 AttributeFlag = AF_None;
};

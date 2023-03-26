#pragma once

#include "DamageDisplayParams.generated.h"

USTRUCT(BlueprintType)
struct DAMAGESYSTEM_API FDamageDisplayParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DamageDisplayParams")
	int32 DamageValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DamageDisplayParams")
	int32 DamageId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DamageDisplayParams")
	bool bIsCritical = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DamageDisplayParams")
	FVector OccuredLocation = FVector::ZeroVector;
};

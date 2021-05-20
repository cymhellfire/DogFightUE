// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "BlastwaveAreaSettings.generated.h"

USTRUCT(BlueprintType)
struct FBlastwaveAreaSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BlastwaveAreaSettings")
	float BlastForceSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BlastwaveAreaSettings")
	float BlastMaxRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BlastwaveAreaSettings")
	float BlastForceMinRatio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BlastwaveAreaSettings")
	float BlastForceExpandSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BlastwaveAreaSettings")
	float RaiseForceRatio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BlastwaveAreaSettings")
	UCurveFloat* BlastForceFalloffCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BlastwaveAreaSettings")
	float StrengthCost;

	FBlastwaveAreaSettings()
	{
		BlastForceSize = 0.f;
		BlastMaxRadius = 0.f;
		BlastForceMinRatio = 0.f;
		BlastForceExpandSpeed = 0.f;
		RaiseForceRatio = 0.5f;
		BlastForceFalloffCurve = nullptr;
		StrengthCost = 0;
	}
};

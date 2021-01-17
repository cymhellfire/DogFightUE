// Dog Fight Game Code By CYM.


#include "Game/DogFightDamageType.h"

UDogFightDamageType::UDogFightDamageType(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DamageCategoryFlag = 0;
	StrengthCost = 0;
	BlastForce = 0;
	BlastForceUpwardRatio = 0;
	BlastForceFalloff = EBlastForceFalloffType::Constant;
	DamageStyleName = TEXT("Default");
}

float UDogFightDamageType::CalculateBlastForceSize(FVector Origin, FVector TargetLocation, float Range) const
{
	if (BlastForceFalloff == EBlastForceFalloffType::Constant)
		return BlastForce;

	const float Distance = (Origin - TargetLocation).Size();
	switch(BlastForceFalloff)
	{
	case EBlastForceFalloffType::Linear:
		return BlastForce * FMath::Clamp(Distance / Range, 0.f, 1.f);
	case EBlastForceFalloffType::Curve:
		return BlastForce * BlastForceFalloffCurve->GetFloatValue(FMath::Clamp(Distance / Range, 0.f, 1.f));
	}

	return BlastForce;
}

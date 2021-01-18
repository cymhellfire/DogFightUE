// Dog Fight Game Code By CYM.

#include "Game/DogFightDamageType.h"
#include "Common/Localization.h"

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

FString UDogFightDamageType::GetDogFightDamageCategoryName(EDogFightDamageCategory Category)
{
	static UEnum* DamageCategoryEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EDogFightDamageCategory"));
	return DamageCategoryEnum->GetNameStringByValue(static_cast<int64>(Category));
}

FText UDogFightDamageType::GetFormattedDamageCategoryName(int32 CategoryFlag)
{
	FFormatOrderedArguments FormatArgumentValues;
	int32 CategoryCount = 0;
	if (TEST_DAMAGE_CATEGORY(CategoryFlag, EDogFightDamageCategory::Damage_Physical))
	{
		FormatArgumentValues.Add(FText::FromStringTable(ST_CARD_LOC, GetDogFightDamageCategoryName(EDogFightDamageCategory::Damage_Physical)));
		CategoryCount++;
	}
	if (TEST_DAMAGE_CATEGORY(CategoryFlag, EDogFightDamageCategory::Damage_Magical))
	{
		FormatArgumentValues.Add(FText::FromStringTable(ST_CARD_LOC, GetDogFightDamageCategoryName(EDogFightDamageCategory::Damage_Magical)));
		CategoryCount++;
	}

	FString FormatString;
	for (int32 i = 0; i < CategoryCount; ++i)
	{
		FormatString.Append(FString::Printf(TEXT("{%d}"), i));
		if (i < CategoryCount - 1)
		{
			FormatString.Append(" / ");
		}
	}

	return FText::Format(FTextFormat::FromString(FormatString), FormatArgumentValues);
}

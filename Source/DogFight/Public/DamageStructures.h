#pragma once

#include "CoreMinimal.h"
#include "Common/Localization.h"
#include "Game/DogFightDamageType.h"
#include "DamageStructures.generated.h"

USTRUCT(BlueprintType)
struct FActorArmor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ActorArmor")
	int32 ArmorValue;

	/** Damage categories this armor can handle. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ActorArmor", meta=(Bitmask, BitmaskEnum = "EDogFightDamageCategory"))
	int32 AntiDamageCategories;

	/** The list of DamageType this armor can handle.(Empty means all.) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ActorArmor")
	TArray<TSubclassOf<UDamageType>> DesiredDamageType;

	/** The instigator of this armor. */
	AActor* Instigator;

	/** Check if this armor can reduce specified damage value. */
	bool IsAgainstDamageType(TSubclassOf<UDamageType> TestDamageType) const
	{
		if (UDogFightDamageType* DogFightDamage = Cast<UDogFightDamageType>(TestDamageType->ClassDefaultObject))
		{
			if (TEST_MULTI_DAMAGE_CATEGORY(DogFightDamage->DamageCategoryFlag, AntiDamageCategories))
			{
				return IsDamageTypeDesired(TestDamageType);
			}
			else
			{
				return false;
			}
		}

		return IsDamageTypeDesired(TestDamageType);
	}

	bool IsDamageTypeDesired(TSubclassOf<UDamageType> TestDamageType) const
	{
		if (DesiredDamageType.Num() > 0)
		{
			return DesiredDamageType.Contains(TestDamageType);
		}

		// If no DamageType is desired, all DamageType is acceptable
		return true;
	}

	FText GetArmorEffectText() const
	{
		FFormatOrderedArguments FinalArguments;
		FinalArguments.Add(FText::FromString(FString::FromInt(ArmorValue)));

		// If desired damage type is specified, use the damage type string
		if (DesiredDamageType.Num() > 0)
		{
			FFormatOrderedArguments DesiredDamageTypeArguments;
			FString FormatString;
			for (int32 Index = 0; Index < DesiredDamageType.Num(); ++Index)
			{
				if (UDogFightDamageType* DogFightDamage = Cast<UDogFightDamageType>(DesiredDamageType[Index]))
				{
					if (DesiredDamageTypeArguments.Num() > 0)
					{
						FormatString.Append(TEXT(" / "));
					}

					DesiredDamageTypeArguments.Add(DogFightDamage->DamageTypeName.GetLocalizeText());
					FormatString.Append(TEXT("%s"));
				}
			}
			// damage type string
			FinalArguments.Add(FText::Format(FTextFormat::FromString(FormatString), DesiredDamageTypeArguments));
		}
		// Use damage category string then
		else
		{
			// Construct damage categories string
			FinalArguments.Add(UDogFightDamageType::GetFormattedDamageCategoryName(AntiDamageCategories));
		}

		return FText::Format(FText::FromStringTable(ST_BUFF_LOC, TEXT("FloatText_ArmorDescFormat")), FinalArguments);
	}

	friend bool operator==(const FActorArmor& Lhs, const FActorArmor& RHS)
	{
		return Lhs.ArmorValue == RHS.ArmorValue
			&& Lhs.AntiDamageCategories == RHS.AntiDamageCategories
			&& Lhs.DesiredDamageType == RHS.DesiredDamageType
			&& Lhs.Instigator == RHS.Instigator;
	}

	friend bool operator!=(const FActorArmor& Lhs, const FActorArmor& RHS)
	{
		return !(Lhs == RHS);
	}
};

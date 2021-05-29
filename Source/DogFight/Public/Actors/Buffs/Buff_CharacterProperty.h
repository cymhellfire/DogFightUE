// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Buffs/BuffBase.h"
#include "Common/PropertyModifyCommon.h"
#include "Buff_CharacterProperty.generated.h"

class AStandardModePlayerCharacter;

UENUM(BlueprintType)
enum class ECharacterProperty : uint8
{
	CP_HealthPoint			UMETA(DisplayName="Health Point"),
	CP_StrengthPoint		UMETA(DisplayName="Strength Point"),
};

USTRUCT(BlueprintType)
struct FCharacterPropertyModifier
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECharacterProperty AffectProperty;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EPropertyModifyMethod ModifyMethod;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ModifyValue;

	void ApplyToCharacter(AStandardModePlayerCharacter* Target) const;

	void RemoveFromCharacter(AStandardModePlayerCharacter* Target) const;

	float CalculateValue(float OrgValue, EPropertyModifyMethod Method, bool bReverse = false) const;
};

/**
 * Buff that has ability to modify owner character properties.
 */
UCLASS()
class DOGFIGHT_API ABuff_CharacterProperty : public ABuffBase
{
	GENERATED_BODY()

protected:
	virtual void ApplyBuff() override;
	virtual void RemoveBuff() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buff")
	TArray<FCharacterPropertyModifier> PropertyModifiers;
};

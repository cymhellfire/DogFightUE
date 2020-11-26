// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DogFightDamageType.generated.h"

#define TEST_DAMAGE_CATEGORY(Bitmask, Bit) (((Bitmask) & (1 << static_cast<uint8>(Bit))) > 0)
#define TEST_MULTI_DAMAGE_CATEGORY(Bitmask, DesiredFlags) (((Bitmask) & (DesiredFlags)) == (DesiredFlags))

UENUM(BlueprintType, meta=(Bitflag))
enum class EDogFightDamageCategory : uint8
{
	Damage_Physical		UMETA(DisplayName = "Physical"),
	Damage_Magical		UMETA(DisplayName = "Magical")
};

UENUM(BlueprintType)
enum class EBlastForceFalloffType : uint8
{
	Constant,
	Linear,
	Curve,
};

/**
 * Base class for all DamageType of DogFight game.
 */
UCLASS(Blueprintable)
class DOGFIGHT_API UDogFightDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UDogFightDamageType(const FObjectInitializer& ObjectInitializer);

	virtual float CalculateBlastForceSize(FVector Origin, FVector TargetLocation, float Range) const;

public:
	/** The bitmask represents the damage category. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DamageType", meta=(Bitmask, BitmaskEnum = "EDogFightDamageCategory"))
	int32 DamageCategoryFlag;

	/** The amount of strength target will lose after hit. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DamageType")
	int32 StrengthCost;

	/** The force apply to physical actors. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DamageType")
	float BlastForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DamageType")
	float BlastForceUpwardRatio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DamageType")
	EBlastForceFalloffType BlastForceFalloff;

	/** The falloff curve to calculate the actual blast force. (Only works with Curve type.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DamageType")
	UCurveFloat* BlastForceFalloffCurve;
};

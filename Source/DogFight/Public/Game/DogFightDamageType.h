// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DogFightDamageType.generated.h"

#define TEST_DAMAGE_CATEGORY(Bitmask, Bit) (((Bitmask) & (1 << static_cast<uint8>(Bit))) > 0)

UENUM(BlueprintType, meta=(Bitflag))
enum class EDogFightDamageCategory : uint8
{
	Damage_Physical		UMETA(DisplayName = "Physical"),
	Damage_Magical		UMETA(DisplayName = "Magical")
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

public:
	/** The bitmask represents the damage category. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DamageType", meta=(Bitmask, BitmaskEnum = "EDogFightDamageCategory"))
	int32 DamageCategoryFlag;
};

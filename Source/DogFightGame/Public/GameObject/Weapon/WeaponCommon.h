#pragma once

#include "WeaponCommon.generated.h"

UENUM(BlueprintType)
enum class EWeaponSlotType : uint8
{
	WST_None			UMETA(DisplayName="None"),
	WST_Primary			UMETA(DisplayName="Primary"),
	WST_Secondary		UMETA(DisplayName="Secondary"),
};

UENUM(BlueprintType)
enum class EWeaponActionInput : uint8
{
	WAI_None			UMETA(DisplayName="None"),
	WAI_Attack			UMETA(DisplayName="Attack"),
	WAI_Defence			UMETA(DisplayName="Defence"),
};

UENUM(BlueprintType)
enum class EWeaponActionTargetType : uint8
{
	WATT_None			UMETA(DisplayName="None"),
	WATT_Actor			UMETA(DisplayName="Actor"),
	WATT_Location		UMETA(DisplayName="Location"),
};

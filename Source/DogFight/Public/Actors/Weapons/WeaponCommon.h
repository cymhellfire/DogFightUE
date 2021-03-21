#pragma once

#include "CoreMinimal.h"
#include "WeaponCommon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WT_None			UMETA(DisplayName="None", Tooltip="No weapon equipped."),
	WT_DualSword	UMETA(DisplayName="Dual Sword", Tooltip="Dual sword equipped."),
};

UENUM(BlueprintType)
enum class EWeaponActionInput : uint8
{
	WAI_None			UMETA(DisplayName="None", Tooltip="Enumeration represents no input for weapon."),
	WAI_LightAttack		UMETA(DisplayName="Light Attack", Tooltip="Light attack input for weapon."),
	WAI_HeavyAttack		UMETA(DisplayName="Heavy Attack", Tooltip="Heavy attack input for weapon."),
	WAI_Dodge			UMETA(DisplayName="Dodge", Tooltip="Dodge input for weapon."),
};

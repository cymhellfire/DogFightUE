#pragma once

#include "CoreMinimal.h"

#define TEST_PLAYER_FLAG(Bitmask, Bit) (((Bitmask) & (1 << static_cast<int32>(Bit))) > 0)
#define TEST_MULTI_PLAYER_FLAG(Bitmask, DesiredFlags) (((Bitmask) & (DesiredFlags)) == (DesiredFlags))

UENUM(BlueprintType)
enum class EAIControllerState : uint8
{
	ACS_Idle			UMETA(DisplayName="Idle"),
	ACS_SelectingCard	UMETA(DisplayName="SelectingCard"),
	ACS_UsingCard		UMETA(DisplayName="UsingCard"),
	ACS_CardFinished	UMETA(DisplayName="CardFinished"),
	ACS_Dead			UMETA(DisplayName="Dead"),
};

UENUM(BlueprintType)
enum class EAIPlayerStyle : uint8
{
	APS_Default			UMETA(DisplayName = "Default"),
	APS_Aggressive		UMETA(DisplayName = "Aggressive"),
	APS_Defensive		UMETA(DisplayName = "Defensive")
};

UENUM(BlueprintType)
enum class EAIPlayerAction : uint8
{
	APA_None			UMETA(DisplayName = "None"),
	APA_Attack			UMETA(DisplayName = "Attack"),
	APA_Defence			UMETA(DisplayName = "Defence"),
	APA_Support			UMETA(DisplayName = "Support")
};

UENUM(Meta = (Bitflags))
enum class EFindPlayerFlags
{
	EFP_Enemy			UMETA(DisplayName = "Enemy"),
	EFP_Ally			UMETA(DisplayName = "Ally"),
	EFP_Self			UMETA(DisplayName = "Self"),
	EFP_Human			UMETA(DisplayName = "HumanPlayer"),
	EFP_AI				UMETA(DisplayName = "AIPlayer"),
	EFP_Alive			UMETA(DisplayName = "Alive"),
	EFP_Dead			UMETA(DisplayName = "Dead")
};

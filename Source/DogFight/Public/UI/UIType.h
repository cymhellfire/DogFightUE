#pragma once

/**
 * Selection Mode Enum Used by CardDisplayWidget
 */
UENUM(BlueprintType)
enum class ECardSelectionMode : uint8
{
	CSM_None				UMETA(DisplayName="None"),
	CSM_SingleNoConfirm		UMETA(DisplayName="Single No Confirm"),
	CSM_MultiWithConfirm	UMETA(DisplayName="Multi With Confirm"),
};
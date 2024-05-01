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
	WAI_Finish			UMETA(DisplayName="Finish"),
};

UENUM(BlueprintType)
enum class EWeaponActionTargetType : uint8
{
	WATT_None			UMETA(DisplayName="None"),
	WATT_Actor			UMETA(DisplayName="Actor"),
	WATT_Location		UMETA(DisplayName="Location"),
};

struct FWeaponActionTarget
{
	FWeaponActionTarget(AActor* InActor)
	{
		TargetType = EWeaponActionTargetType::WATT_Actor;
		Actor = InActor;
	}

	FWeaponActionTarget(const FVector& InLocation)
	{
		TargetType = EWeaponActionTargetType::WATT_Location;
		Location = InLocation;
	}

	EWeaponActionTargetType TargetType;
	union
	{
		AActor* Actor;
		FVector Location;
	};

	AActor* GetActorTarget() const
	{
		if (TargetType == EWeaponActionTargetType::WATT_Actor)
		{
			return Actor;
		}
		return nullptr;
	}

	FVector GetLocationTarget() const
	{
		if (TargetType == EWeaponActionTargetType::WATT_Location)
		{
			return Location;
		}
		return FVector::ZeroVector;
	}

	TOptional<FVector> AsLocation() const
	{
		switch (TargetType)
		{
		case EWeaponActionTargetType::WATT_Actor:
			return IsValid(Actor) ? Actor->GetActorLocation() : TOptional<FVector>();
		case EWeaponActionTargetType::WATT_Location:
			return Location;
		default:
			return TOptional<FVector>();
		}
	}
};

#pragma once

#include "DogFightTypes.generated.h"

UENUM()
namespace EReturnToMainMenuReason
{
	enum Type
	{
		Unknown			UMETA(DisplayName="NetError_Unknown"),
		LostConnect		UMETA(DisplayName="NetError_LostConnect"),
		HostLeft		UMETA(DisplayName="NetError_HostLeft"),
	};
}

UENUM(BlueprintType)
enum class EGameMessageType : uint8
{
	GMT_Default			UMETA(DisplayName="Default"),
	GMT_System			UMETA(DisplayName="System"),
	GMT_Player			UMETA(DisplayName="Player"),
};

USTRUCT(BlueprintType)
struct FGameMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString Source;

	UPROPERTY(BlueprintReadOnly)
	EGameMessageType Type;

	UPROPERTY(BlueprintReadOnly)
	FString MessageString;

	UPROPERTY(BlueprintReadOnly)
	TArray<FText> Arguments;
};

USTRUCT(BlueprintType)
struct FGameTitleMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString MessageString;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> Arguments;
};

UENUM(BlueprintType)
enum class EAIControllerState : uint8
{
	ACS_Idle		UMETA(DisplayName="Idle"),
	ACS_UsingCard	UMETA(DisplayName="UsingCard"),
};

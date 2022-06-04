#pragma once

#include "DogFightTypes.generated.h"

UENUM(BlueprintType)
enum class EGameMessageType : uint8
{
	GMT_Default			UMETA(DisplayName="Default"),
	GMT_System			UMETA(DisplayName="System"),
	GMT_Player			UMETA(DisplayName="Player"),
};

namespace ECameraFocusEventType
{
	enum Type
	{
		Default,			// Default events that only listening players response to
		OwnerForced,		// Events that let owner player must response to
	};
}

USTRUCT()
struct FCameraFocusEvent
{
	GENERATED_BODY();

	int32 OwnerPlayerId;
	double LocationX;
	double LocationY;

	ECameraFocusEventType::Type EventType;
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

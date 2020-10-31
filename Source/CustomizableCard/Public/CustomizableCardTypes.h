#pragma once

#include "CustomizableCardTypes.generated.h"

UENUM(BlueprintType)
enum class ECardInstructionExecuteType : uint8
{
	Synchronous,
	Asynchronous
};

UENUM(BlueprintType)
enum class ECardInstructionTargetType : uint8
{
	Actor,
	Position,
	Direction
};

USTRUCT(BlueprintType)
struct FCardInstructionTargetInfo
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetInfo")
	ECardInstructionTargetType TargetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetInfo")
	AActor* ActorPtr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetInfo")
	FVector PositionValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetInfo")
	FVector DirectionValue;

	friend bool operator==(const FCardInstructionTargetInfo& Lhs, const FCardInstructionTargetInfo& RHS)
	{
		return Lhs.TargetType == RHS.TargetType
			&& Lhs.ActorPtr == RHS.ActorPtr
			&& Lhs.PositionValue == RHS.PositionValue
			&& Lhs.DirectionValue == RHS.DirectionValue;
	}

	friend bool operator!=(const FCardInstructionTargetInfo& Lhs, const FCardInstructionTargetInfo& RHS)
	{
		return !(Lhs == RHS);
	}
};

USTRUCT(BlueprintType)
struct FCardDisplayInfoArgument
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CardDisplayArgument")
	FColor DisplayColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CardDisplayArgument")
	FString StringValue;
};

USTRUCT(BlueprintType)
struct FCardInstanceDisplayInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CardInstanceInfo")
	FString CardName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CardInstanceInfo")
	FString CardDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CardInstanceInfo")
	TArray<FCardDisplayInfoArgument> Arguments;
};

/** Test if all desired flags are toggled on. */
#define TEST_MULTI_CARD_CATEGORY(Bitmask, DesiredFlags) (((Bitmask) & (DesiredFlags)) == (DesiredFlags))

UENUM(Meta = (Bitflags))
enum class ECardCategoryFlags
{
	ECC_Attack			UMETA(DisplayName = "Attack"),
	ECC_Defence			UMETA(DisplayName = "Defence"),
	ECC_Support			UMETA(DisplayName = "Support")
};

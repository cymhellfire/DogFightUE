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

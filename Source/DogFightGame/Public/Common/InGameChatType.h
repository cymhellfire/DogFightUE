#pragma once

#include "InGameChatType.generated.h"

UENUM(BlueprintType)
namespace EInGameChatSourceType
{
	enum Type
	{
		System,
		Player,
	};
}

USTRUCT(BlueprintType)
struct FInGameChatMessage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InGameChatMessage")
	TEnumAsByte<EInGameChatSourceType::Type> SourceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InGameChatMessage")
	int32 SourcePlayerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InGameChatMessage")
	FText Content;
};

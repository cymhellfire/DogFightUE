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

USTRUCT(BlueprintType)
struct FInGameChatSendOption
{
	GENERATED_BODY()

	/**
	 * Id of player who sent this message.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InGameChatSendOption")
	int32 SourcePlayerId;

	/**
	 * Id of player who is expected to receive this message. (-1 means broadcast to all players.)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InGameChatSendOption")
	int32 TargetPlayerId = -1;

	/**
	 * Message content to send.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InGameChatSendOption")
	FText Content;
};

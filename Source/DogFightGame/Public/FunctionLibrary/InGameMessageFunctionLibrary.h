#pragma once

#include "Common/InGameChatType.h"
#include "InGameMessageFunctionLibrary.generated.h"

class UInGameMessageReceiverComponent;

/**
 * Provide all functions related to in-game message.
 */
UCLASS()
class DOGFIGHTGAME_API UInGameMessageFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category="InGameMessageFunction")
	static void SetTitleMessage(const FText& InText);

	/**
	 * Send a in-game chat message to all players in current game.
	 * @param ChatMessage			Message to send.
	 */
	UFUNCTION(BlueprintCallable, Category="InGameMessageFunction")
	static void SendInGameChatMessage(const FInGameChatMessage& ChatMessage);

	/**
	 * Get the in-game message receiver component of local player.
	 */
	UFUNCTION(BlueprintCallable, Category="InGameMessageFunction")
	static UInGameMessageReceiverComponent* GetLocalPlayerMessageReceiver();
};

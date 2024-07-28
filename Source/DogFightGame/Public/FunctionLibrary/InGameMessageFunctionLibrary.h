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
	
	UFUNCTION(BlueprintCallable, Category="InGameMessageFunction", meta=(WorldContext="WorldContextObject"))
	static void SetTitleMessage(UObject* WorldContextObject, const FText& InText);

	/**
	 * Send a in-game chat message to other players in current game as player.
	 * @param SendOption			Message send option.
	 */
	UFUNCTION(BlueprintCallable, Category="InGameMessageFunction", meta=(WorldContext="WorldContextObject"))
	static void SendInGameChatMessageAsPlayer(UObject* WorldContextObject, const FInGameChatSendOption& SendOption);

	/**
	 * Send a in-game chat message to other players in current game as system. (Only available on server side.)
	 * @param SendOption			Message send option.
	 */
	UFUNCTION(BlueprintCallable, Category="InGameMessageFunction", meta=(WorldContext="WorldContextObject"))
	static void SendInGameChatMessageAsSystem(UObject* WorldContextObject, const FInGameChatSendOption& SendOption);

	/**
	 * Get the in-game message receiver component of local player.
	 */
	UFUNCTION(BlueprintCallable, Category="InGameMessageFunction", meta=(WorldContext="WorldContextObject"))
	static UInGameMessageReceiverComponent* GetLocalPlayerMessageReceiver(UObject* WorldContextObject);
};

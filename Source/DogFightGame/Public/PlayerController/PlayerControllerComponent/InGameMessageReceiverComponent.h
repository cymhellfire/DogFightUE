#pragma once

#include "CoreMinimal.h"
#include "Common/InGameChatType.h"
#include "InGameMessageReceiverComponent.generated.h"

UCLASS()
class DOGFIGHTGAME_API UInGameMessageReceiverComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UInGameMessageReceiverComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(NetMulticast, Reliable)
	void ReceiveMessage(const FText& Message);

	UFUNCTION(NetMulticast, Reliable)
	void ReceiveInGameChatMessage(const FInGameChatMessage& ChatMessage);

	UFUNCTION(BlueprintCallable, Category="InGameMessageReceiver")
	bool GetCachedGameChatMessage(FInGameChatMessage& OutMessage);

private:
	TOptional<FInGameChatMessage> CachedChatMessage;
};

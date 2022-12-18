#pragma once

#include "CoreMinimal.h"
#include "InGameMessageReceiverComponent.generated.h"

UCLASS()
class DOGFIGHTGAME_API UInGameMessageReceiverComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UInGameMessageReceiverComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(NetMulticast, Reliable)
	void ReceiveMessage(const FText& Message);
};

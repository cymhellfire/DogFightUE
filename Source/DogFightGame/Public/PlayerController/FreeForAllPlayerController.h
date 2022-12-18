#pragma once

#include "Player/DogFightPlayerController.h"
#include "FreeForAllPlayerController.generated.h"

class UInGameMessageReceiverComponent;

UCLASS()
class DOGFIGHTGAME_API AFreeForAllPlayerController : public ADogFightPlayerController
{
	GENERATED_BODY()
public:
	AFreeForAllPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	UInGameMessageReceiverComponent* GetInGameMessageReceiverComponent() const
	{
		return InGameMessageReceiverComponent;
	}

protected:
	UFUNCTION(Server, Reliable)
	void ServerMarkPlayerReady();

	UFUNCTION()
	void OnTimerExpired();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerController")
	UInGameMessageReceiverComponent* InGameMessageReceiverComponent;

protected:
	FTimerHandle RandomTimer;
};

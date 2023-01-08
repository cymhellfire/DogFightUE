#pragma once

#include "GameMode/GameFlowBasedGameMode.h"
#include "TopDownStyleGameMode.generated.h"

class ATopDownStylePlayerController;
class UInGameMessageSenderComponent;

UCLASS()
class DOGFIGHTGAME_API ATopDownStyleGameMode : public AGameFlowBasedGameMode
{
	GENERATED_BODY()
public:
	ATopDownStyleGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void PlayerReadyForGame(ATopDownStylePlayerController* InPC);

	TArray<TWeakObjectPtr<ATopDownStylePlayerController>> GetAllPlayerControllers() const
	{
		return AllPlayerControllers;
	}

	UInGameMessageSenderComponent* GetInGameMessageSender() const
	{
		return InGameMessageSenderComponent;
	}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GameMode")
	UInGameMessageSenderComponent* InGameMessageSenderComponent;

	/* All player controllers in current game. */
	TArray<TWeakObjectPtr<ATopDownStylePlayerController>> AllPlayerControllers;

	int32 ReadyPlayerCount;
};

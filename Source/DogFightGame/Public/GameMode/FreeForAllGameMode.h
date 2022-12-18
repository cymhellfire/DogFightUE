#pragma once

#include "GameMode/GameFlowBasedGameMode.h"
#include "FreeForAllGameMode.generated.h"

class AFreeForAllPlayerController;
class UInGameMessageSenderComponent;

UCLASS()
class DOGFIGHTGAME_API AFreeForAllGameMode : public AGameFlowBasedGameMode
{
	GENERATED_BODY()
public:
	AFreeForAllGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void PlayerReadyForGame(AFreeForAllPlayerController* InPC);

	TArray<TWeakObjectPtr<AFreeForAllPlayerController>> GetAllPlayerControllers() const
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
	TArray<TWeakObjectPtr<AFreeForAllPlayerController>> AllPlayerControllers;

	int32 ReadyPlayerCount;
};

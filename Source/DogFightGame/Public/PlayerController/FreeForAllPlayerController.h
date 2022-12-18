#pragma once

#include "CoreMinimal.h"
#include "Player/DogFightPlayerController.h"
#include "FreeForAllPlayerController.generated.h"

class AFreeForAllPlayerCharacter;
class UInGameMessageReceiverComponent;

UCLASS()
class DOGFIGHTGAME_API AFreeForAllPlayerController : public ADogFightPlayerController
{
	GENERATED_BODY()
public:
	AFreeForAllPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SpawnCharacterPawn();

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

	UPROPERTY(Replicated, Transient)
	AFreeForAllPlayerCharacter* CharacterPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerController")
	TSubclassOf<AFreeForAllPlayerCharacter> CharacterClass;

protected:
	FTimerHandle RandomTimer;
};

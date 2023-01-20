#pragma once

#include "CoreMinimal.h"
#include "Common/InputMappingDef.h"
#include "Player/CardTargetProviderInterface.h"
#include "Player/DogFightPlayerController.h"
#include "TopDownStylePlayerController.generated.h"

class AFreeForAllPlayerCharacter;
class UInGameMessageReceiverComponent;
class UCardTargetProviderComponent;
class UInputAction;

UCLASS()
class DOGFIGHTGAME_API ATopDownStylePlayerController : public ADogFightPlayerController, public ICardTargetProviderInterface
{
	GENERATED_BODY()
public:
	ATopDownStylePlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetupInputComponent() override;

	// -----=========== Input ===========-----
	UFUNCTION(Client, Reliable)
	void ClientAddInputMapping(EInputMappingType::Type InputType);

	UFUNCTION(Client, Reliable)
	void ClientRemoveInputMapping(EInputMappingType::Type InputType);

	// -----=========== Character ===========-----
	void SpawnCharacterPawn();

	// -----=========== Card ===========-----
	UFUNCTION(Server, Reliable)
	void ServerUseCardByInstanceId(int32 InId);

	UInGameMessageReceiverComponent* GetInGameMessageReceiverComponent() const
	{
		return InGameMessageReceiverComponent;
	}

	AFreeForAllPlayerCharacter* GetCharacterPawn() const
	{
		return CharacterPawn;
	}

	// ICardTargetProviderInterface interface
	virtual void StartAcquireTargets(FTargetAcquireSettings Settings, TFunction<void(bool bSuccess, TArray<FAcquiredTargetInfo>)> Callback) override;

protected:
	UFUNCTION(Server, Reliable)
	void ServerMarkPlayerReady();

	UFUNCTION()
	void OnTimerExpired();

	UFUNCTION()
	void OnCardTargetAcquired(bool bSuccess);

	UFUNCTION(Client, Reliable)
	void ClientStartAcquireTargets(FTargetAcquireSettings Settings);

	UFUNCTION(Server, Reliable)
	void ServerFinishAcquireTargets(bool bSuccess, const TArray<FAcquiredTargetInfo>& TargetInfos);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerController")
	UInGameMessageReceiverComponent* InGameMessageReceiverComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerController")
	UCardTargetProviderComponent* CardTargetProviderComponent;

	UPROPERTY(Replicated, Transient)
	AFreeForAllPlayerCharacter* CharacterPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerController")
	TSubclassOf<AFreeForAllPlayerCharacter> CharacterClass;

protected:
	FTimerHandle RandomTimer;

	TFunction<void(bool, TArray<FAcquiredTargetInfo>)> AcquireTargetCallback;
};

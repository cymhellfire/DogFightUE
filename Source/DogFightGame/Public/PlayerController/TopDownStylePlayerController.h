#pragma once

#include "CoreMinimal.h"
#include "Common/InGameChatType.h"
#include "Common/InputMappingDef.h"
#include "Player/CardTargetProviderInterface.h"
#include "Player/DogFightPlayerController.h"
#include "TopDownStylePlayerController.generated.h"

class ATopDownStylePlayerCharacter;
class UInGameMessageReceiverComponent;
class UInGameWidgetManipulatorComponent;
class UInGameUIInteractComponent;
class UCardTargetProviderComponent;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTargetAcquiredSignature, bool, bSucceed, TArray<FAcquiredTargetInfo>, TargetInfos);

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
	void ClientAddInputMapping(EInputMappingType::Type InputType, EInputMappingPriority::Type Priority);

	UFUNCTION(Client, Reliable)
	void ClientRemoveInputMapping(EInputMappingType::Type InputType);

	// -----=========== Character ===========-----
	void SpawnCharacterPawn();

	// -----=========== Card ===========-----
	UFUNCTION(Server, Reliable)
	void ServerUseCardByInstanceId(int32 InId);

	// -----=========== Effect ===========-----
	UFUNCTION(Client, Reliable)
	void ClientSpawnGameEffectAtPos(int32 EffectId, FVector Pos, FRotator Rot);

	// -----=========== Chat ===========-----
	UFUNCTION(Server, Reliable)
	void ServerSendInGameChatMessage(const FInGameChatMessage& InMessage);

	UInGameMessageReceiverComponent* GetInGameMessageReceiverComponent() const
	{
		return InGameMessageReceiverComponent;
	}

	UInGameWidgetManipulatorComponent* GetInGameWidgetManipulatorComponent() const
	{
		return InGameWidgetManipulatorComponent;
	}

	UFUNCTION(BlueprintCallable, Category="TopDownStylePlayerController")
	ATopDownStylePlayerCharacter* GetCharacterPawn() const
	{
		return CharacterPawn;
	}

	// ICardTargetProviderInterface interface
	virtual void StartAcquireTargets(FTargetAcquireSettings Settings, TFunction<void(bool bSuccess, TArray<FAcquiredTargetInfo>)> Callback) override;

	UFUNCTION(BlueprintCallable, Category="TopDownStylePlayerController")
	virtual void StartAcquireTargetsWithoutCallback(FTargetAcquireSettings Settings);

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
	UInGameWidgetManipulatorComponent* InGameWidgetManipulatorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerController")
	UInGameUIInteractComponent* InGameUIInteractComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerController")
	UCardTargetProviderComponent* CardTargetProviderComponent;

	UPROPERTY(Replicated, Transient)
	ATopDownStylePlayerCharacter* CharacterPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerController")
	TSubclassOf<ATopDownStylePlayerCharacter> CharacterClass;

public:
	UPROPERTY(BlueprintAssignable, Category="TopDownStylePlayerController")
	FOnTargetAcquiredSignature OnTargetAcquired;

protected:
	FTimerHandle RandomTimer;

	TFunction<void(bool, TArray<FAcquiredTargetInfo>)> AcquireTargetCallback;
};

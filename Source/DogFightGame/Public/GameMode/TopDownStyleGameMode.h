#pragma once

#include "DamageType/ExtendedDamageEvent.h"
#include "GameMode/GameFlowBasedGameMode.h"
#include "Player/TopDownStylePlayerState.h"
#include "TopDownStyleGameMode.generated.h"

class ATopDownStylePlayerController;
class UInGameMessageSenderComponent;
class UGameTimelineComponent;
class UDamageCalculatorBase;
class ATopDownStylePlayerState;

UCLASS()
class DOGFIGHTGAME_API ATopDownStyleGameMode : public AGameFlowBasedGameMode
{
	GENERATED_BODY()
public:
	ATopDownStyleGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	void PlayerReadyForGame(ATopDownStylePlayerController* InPC);

	void PlayerRequestFinishRound(ATopDownStylePlayerController* InPC);

	TArray<TWeakObjectPtr<ATopDownStylePlayerController>> GetAllPlayerControllers() const
	{
		return AllPlayerControllers;
	}

	UInGameMessageSenderComponent* GetInGameMessageSender() const
	{
		return InGameMessageSenderComponent;
	}

	UGameTimelineComponent* GetGameTimelineComponent() const;

	// ----------------- Gameplay Section ---------------------
	/**
	 * @brief Switch the character movement on/off for all players in current game.
	 * @param bEnable Whether to enable character movement.
	 */
	void SetEnableCharacterMoveForAllPlayers(bool bEnable);

	/**
	 * Apply damage to target actor.
	 * @param DamageId			Id of damage config.
	 * @param Target			Target actor that damage apply to.
	 * @param BaseDamage		Original damage value.
	 * @param Causer			Causer of this damage.
	 */
	void DamageActor(int32 DamageId, AActor* Target, float BaseDamage, AActor* Causer);

	/**
	 * Apply damage to all actors inside area.
	 * @param DamageId			Id of damage config.
	 * @param Origin			Target location.
	 * @param Radius			Sphere radius.
	 * @param BaseDamage		Original damage value.
	 * @param Causer			Causer of this damage.
	 */
	void DamageArea(int32 DamageId, const FVector& Origin, float Radius, float BaseDamage, AActor* Causer);

protected:
	UFUNCTION()
	void OnDamageEventOccured(class UExtendedDamageInstance* DamageInstance, const FExtendedDamageEvent& DamageEvent);

	void OnAnyPlayerStateChanged(ATopDownStylePlayerState* PlayerState, ETopDownStylePlayerState::Type InState);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TopDownStyleGameMode")
	FString DamageCalculatorPath;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GameMode")
	UInGameMessageSenderComponent* InGameMessageSenderComponent;

	UPROPERTY(Transient)
	UDamageCalculatorBase* DamageCalculator;

	/* All player controllers in current game. */
	TArray<TWeakObjectPtr<ATopDownStylePlayerController>> AllPlayerControllers;

	int32 ReadyPlayerCount;
};

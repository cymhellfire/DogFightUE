// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "DogFightGameModeBase.generated.h"

class UDamageCalculatorBase;
class AShieldManager;
class UGameModeStateMachine;

UENUM(BlueprintType)
enum class EPlayerRelation : uint8
{
	PR_Enemy,
	PR_Ally,
};

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ADogFightGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginDestroy() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual void PreInitializeComponents() override;

	virtual void PostInitializeComponents() override;

	/** Finish current game and pump all players to main menu. */
	virtual void RequestFinishAndExitToMainMenu();

	/** Notify all clients that game will start. */
	virtual void NotifyClientGameWillStart();

	virtual float CalculateDamage(AActor* DamageTaker, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	virtual EPlayerRelation GetPlayersRelation(AController* PlayerA, AController* PlayerB);

	virtual AShieldManager* GetShieldManager() const { return ShieldManager; }

	virtual UGameModeStateMachine* GetGameModeStateMachine() { return GameModeStateMachine; }

protected:

	virtual void BeginPlay() override;

	virtual void InitializeStateMachine() {}

protected:
	
	/** List of all player controller in current game. */
	TArray<class ADogFightPlayerController*> PlayerControllerList;

	/** Class to calculate the damage to apply during game. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DogFightGameMode")
	TSubclassOf<UDamageCalculatorBase> DamageCalculatorClass;

	UPROPERTY()
	UDamageCalculatorBase* DamageCalculator;

	UPROPERTY()
	AShieldManager* ShieldManager;

	UPROPERTY(Transient)
	UGameModeStateMachine* GameModeStateMachine;
};

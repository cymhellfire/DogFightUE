// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "DogFightGameModeBase.generated.h"

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
class PROJECTFRAMEWORK_API ADogFightGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	/** Finish current game and pump all players to main menu. */
	virtual void RequestFinishAndExitToMainMenu();

	/** Notify all clients that game will start. */
	virtual void NotifyClientGameWillStart();

	virtual float CalculateDamage(AActor* DamageTaker, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	virtual void ApplyDamageTo(AActor* DamagedActor, float BaseDamage, FName DamageTypeName, AActor* DamageCauser, AController* DamageInstigator) {}

	virtual EPlayerRelation GetPlayersRelation(AController* PlayerA, AController* PlayerB);

protected:

	virtual void BeginPlay() override;

protected:
	
	/** List of all player controller in current game. */
	TArray<class ADogFightPlayerController*> PlayerControllerList;
};

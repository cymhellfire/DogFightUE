#pragma once

#include "DamageType/ExtendedDamageEvent.h"
#include "GameService/LuaGameService.h"
#include "Interface/ActorPoolInterface/ActorPoolInterface.h"
#include "DamageService.generated.h"

class UDamageCalculatorBase;
class UDamageDisplayWidget;
class UExtendedDamageInstance;
class ADamageDisplayActor;

UCLASS()
class DOGFIGHTGAME_API UDamageService : public ULuaGameService, public IActorPoolInterface
{
	GENERATED_BODY()

public:

	/**
	 * Create a new damage calculator with given class and script.
	 * @param InClass		Class of new damage calculator.
	 * @param ScriptPath	Path of script to bind.
	 * @param Instigator	Instigator actor of this creation.
	 * @return New damage calculator instance.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="DamageService")
	UDamageCalculatorBase* CreateDamageCalculator(TSubclassOf<UDamageCalculatorBase> InClass, const FString& ScriptPath, AActor* Instigator);

	/**
	 * Apply damage to target actor.
	 * @param DamageId		Id of damage config.
	 * @param Target		Target actor.
	 * @param BaseDamage	Initial value of damage.
	 * @param Causer		Causer of this damage.
	 * @param InCalculator	Damage calculator instance to use.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="DamageService")
	void ApplyDamageToActor(int32 DamageId, AActor* Target, float BaseDamage, AActor* Causer, UDamageCalculatorBase* InCalculator);

	/**
	 * Create a damage display actor with given data.
	 * @param DamageInstance		Damage instance of this event.
	 * @param DamageEvent			Damage event data.
	 */
	UFUNCTION(BlueprintCallable, Category="DamageService")
	void CreateDamageDisplay(UExtendedDamageInstance* DamageInstance, const FExtendedDamageEvent& DamageEvent);

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.DamageService.DamageService";
	}

protected:
	UFUNCTION(BlueprintCallable, Category="DamageService")
	void RegisterDamageType(int32 Id, UExtendedDamageInstance* Instance);

	UFUNCTION(BlueprintCallable, Category="DamageService")
	UExtendedDamageInstance* GetDamageInstanceById(int32 InId) const;

	UFUNCTION(BlueprintImplementableEvent, Category="DamageService")
	UDamageDisplayWidget* CreateDamageWidget();

	UFUNCTION()
	void OnDamageDisplayActorDead(ADamageDisplayActor* Instance);

protected:
	UPROPERTY(Transient)
	TMap<int32, UExtendedDamageInstance*> DamageTypeMap;
};

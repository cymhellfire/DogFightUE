// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponModelBase.generated.h"

class ACharacter;

DECLARE_MULTICAST_DELEGATE_ThreeParams(FWeaponModelHitEvent, AActor*, UPrimitiveComponent*, const FHitResult&);

/**
 * Actor class defines weapon looking and features.
 */
UCLASS(Blueprintable, Category="ActionGameWeapon")
class ACTIONGAME_API AWeaponModelBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponModelBase();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Set attack detecting enable or not.
	 * @param bEnable			Whether start attack detecting.
	 */
	virtual void SetAttackDetectEnable(bool bEnable);

	/**
	 * Set the owner character of this weapon.
	 * @param InOwner			Character instance set as owner.
	 */
	void SetOwnerCharacter(ACharacter* InOwner);

	/**
	 * Set the component to use as damage detecting one.
	 * @param InComponent		Component to do damage detect.
	 */
	void SetAttackDetectComponent(UPrimitiveComponent* InComponent);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Weapon hit target.
	 */
	virtual void OnHitTarget(AActor* TargetActor, UPrimitiveComponent* TargetComponent, const FHitResult& HitResult);

	void UnbindAttackDetectComponent();

	UFUNCTION()
	void OnAttackDetectingOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	FWeaponModelHitEvent HitEvent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponModel")
	FName DetectComponentName;

private:
	uint8 bAttackDetecting : 1;
	uint8 bDetectComponentValid : 1;

	TWeakObjectPtr<ACharacter> OwnerCharacter;
	TWeakObjectPtr<UPrimitiveComponent> DetectComponent;
};

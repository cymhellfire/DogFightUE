// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponMeshActor.generated.h"

UCLASS()
class DOGFIGHT_API AWeaponMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponMeshActor();

	void SetWeaponMeshOwner(ACharacter* OwnerCharacter);

	void SetDetectHit(bool bActivate);

	void SetDamageRatio(float NewDamageRatio);

	float GetBaseDamage() const { return WeaponDamage; }

protected:
	UFUNCTION()
	void OnMeshComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponMeshActor")
	float WeaponDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponMeshActor")
	float StrengthCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponMeshActor")
	TSubclassOf<UDamageType> WeaponDamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponMeshActor")
	FName HitDetectCollisionProfile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WeaponMeshActor")
	UStaticMeshComponent* StaticMeshComponent;

	bool bDetectHit;
	float DamageRatio;

	FName DefaultCollisionProfile;

	AActor* WeaponOwner;
	AController* WeaponOwnerController;

	TArray<AActor*> HitActorList;
};

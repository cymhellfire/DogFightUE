// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageArea.generated.h"

UCLASS()
class DOGFIGHT_API ADamageArea : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ADamageArea();

	void InitializeDamageArea(float InDamage, float InDamageRadius, TSubclassOf<UDamageType> InDamageType);

	void SetRelationship(AController* NewOwnerController, AActor* InInstigator);

	void SetStrengthCost(float InStrengthCost);

	void Activate(float Delay = 0);

protected:
	void ApplyDamage();

	UFUNCTION()
	void OnDelayTimerExpired();

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDamageAreaDelegateWithPointer, ADamageArea*, DamageArea);
	FDamageAreaDelegateWithPointer OnDamageAreaFinished;

protected:
	float Damage;
	float DamageRadius;
	float StrengthCost;
	TSubclassOf<UDamageType> DamageType;
	AController* OwnerController;
	AActor* DamageInstigator;

	FTimerHandle DelayTimerHandle;
};

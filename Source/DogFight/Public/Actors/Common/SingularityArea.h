// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SingularityArea.generated.h"

class AStandardModePlayerCharacter;

UCLASS()
class DOGFIGHT_API ASingularityArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASingularityArea();

	void InitializeSingularityArea(float InDragForceSize, float MaxDistance, float MinDistance, float InDuration, float InMinDragForceRatio = 0.f);

	void SetupStrengthCost(int32 InStrengthCostPerTick, float InTickInterval);

	void SetupDamageRelative(TSubclassOf<UDamageType> InDamageType, AActor* InDamageInstigator);

	void Active();

	virtual void Tick(float DeltaSeconds) override;

protected:

	FVector CalculateDragForce(FVector Pos);

	UFUNCTION()
	void OnActorStartOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	void OnActorEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnRep_DragForceMaxDistance();

protected:
	UPROPERTY(Replicated)
	uint8 bActive : 1;
	uint8 bCostStrength : 1;

	UPROPERTY(Replicated)
	float DragForceSize;
	UPROPERTY(Replicated)
	float MinDragForceRatio;
	UPROPERTY(ReplicatedUsing=OnRep_DragForceMaxDistance)
	float DragForceMaxDistance;

	/** Objects in this range will not be drag toward center. */
	UPROPERTY(Replicated)
	float DragForceMinDistance;
	UPROPERTY(Replicated)
	float RaiseForceRatio;
	UPROPERTY(Replicated)
	float SingularityAreaDuration;
	float SingularityAreaTimer;

	float StrengthCostPerTick;
	float StrengthCostTickInterval;
	float StrengthCostTickTimer;

	TSubclassOf<class UDogFightDamageType> DamageType;
	AActor* DragInstigator;
	TArray<UPrimitiveComponent*> DraggingComponentList;
	TMap<AStandardModePlayerCharacter*, int32> DraggingCharacterMap;

	TArray<AStandardModePlayerCharacter*> CharacterToRemove;
};

// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlastwaveArea.generated.h"

struct FBlastwaveAreaSettings;
class AStandardModePlayerCharacter;

struct FBlastwaveAffectComponent
{
	UPrimitiveComponent* AffectComponent;
	float Delay;
	FVector BlastForce;

	void ApplyBlastForce() const;
};

struct FBlastwaveAffectCharacter
{
	AStandardModePlayerCharacter* AffectCharacter;
	float Delay;
	FVector BlastForce;
	int32 StrengthCost;

	void ApplyBlastForce() const;
};

UCLASS()
class DOGFIGHT_API ABlastwaveArea : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlastwaveArea();

	void InitializeBlastwaveArea(FBlastwaveAreaSettings NewSettings);

	void SetIgnoreActors(TArray<AActor*> Actors);

	void Active();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastActive();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TTuple<FVector, float> CalculateBlastForceSize(FVector TargetPos) const;

	void FilterOverlappedRecord();

	UFUNCTION()
	void OnActorStartOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	void OnActorEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnRep_BlastForceMaxRadius();

protected:
	UPROPERTY(Replicated)
	float BlastForceSize;
	UPROPERTY(Replicated)
	float BlastForceMinRatio;
	UPROPERTY(ReplicatedUsing=OnRep_BlastForceMaxRadius)
	float BlastForceMaxRadius;
	UPROPERTY(Replicated)
	float BlastForceExpandSpeed;
	UPROPERTY(Replicated)
	float RaiseForceRatio;

	UPROPERTY(Replicated)
	UCurveFloat* BlastForceFalloffCurve; 

	float StrengthCost;
	float ActiveTimer;

	bool bActive;

	TArray<UPrimitiveComponent*> AffectComponentList;
	TMap<AStandardModePlayerCharacter*, int32> AffectCharacterMap;

	TArray<FBlastwaveAffectComponent> AffectComponentQueue;
	TArray<FBlastwaveAffectCharacter> AffectCharacterQueue;

	TArray<AActor*> IgnoreActors;
};

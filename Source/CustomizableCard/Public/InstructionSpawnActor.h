// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/HandleTargetInstructionBase.h"
#include "InstructionSpawnActor.generated.h"

UENUM(BlueprintType)
enum class EActorDestructionType : uint8
{
	ADT_AutoWithCard		UMETA(DisplayName="Auto With Card", Tooltip="Auto destruct all actors when card finished."),
	ADT_Lifespan			UMETA(DisplayName="Lifespan", Tooltip="Set lifespan for each spawned actor."),
	ADT_NoAction			UMETA(DisplayName="No Action", Tooltip="Do nothing about the spawned actors.")
};

UENUM(BlueprintType)
enum class EAttachRulesType : uint8
{
	ART_KeepWorld,
	ART_KeepRelative,
	ART_SnapToTarget,
};

/**
 * 
 */
UCLASS()
class CUSTOMIZABLECARD_API UInstructionSpawnActor : public UHandleTargetInstructionBase
{
	GENERATED_BODY()

public:
	UInstructionSpawnActor(const FObjectInitializer& ObjectInitializer);

	virtual void BeginDestroy() override;
protected:
	virtual void HandleActorTarget(AActor* Target) override;
	virtual void HandlePositionTarget(FVector Position) override;
	virtual void HandleDirectionTarget(FVector Direction) override;

	virtual void OnHandledAllTarget() override;

	void SpawnActorAt(FVector SpawnLocation, FRotator SpawnRotator);

	void AttachActorsToOwner();

	UFUNCTION()
	void OnDelayAttachTimerExpired();

	UFUNCTION()
	void OnAutoDetachTimerExpired();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	TSubclassOf<AActor> ActorClass;

	/** Use rotation calculated by spawning location and card user's location. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	uint8 bAutoCalculateRotation : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction", meta=(EditCondition="!bAutoCalculateRotation"))
	FRotator InitialRotator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	EActorDestructionType ActorDestructionType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction", meta=(EditCondition="ActorDestructionType==EActorDestructionType::ADT_Lifespan"))
	float Lifespan;

	/** Forcibly treat all target as directional target. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	uint8 bForceUseDirectionSpawn : 1;

	/** The spawn distance among given direction. (Only works with DirectionTarget.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	float SpawnDistance;

	/** The offset (local space) apply to calculated location based on direction and distance. (Only works with DirectionTarget.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FVector SpawnOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	uint8 bAttachToCardUser : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	FName AttachSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	float AttachDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	EAttachRulesType AttachType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	uint8 bAutoDetach : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction", meta=(EditCondition="bAutoDetach"))
	float AutoDetachDelay;

protected:

	UPROPERTY()
	TArray<AActor*> SpawnedActors;

	FTimerHandle DelayAttachTimerHandle;

	FTimerHandle AutoDetachTimerHandle;
};

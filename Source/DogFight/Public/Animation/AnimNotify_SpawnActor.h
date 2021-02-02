// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SpawnActor.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UAnimNotify_SpawnActor : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_SpawnActor();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
protected:

	void SpawnActor(FVector SpawnLocation, FRotator SpawnRotation);

	UFUNCTION()
	void AttachToOwner();

	UFUNCTION()
	void DetachFromOwner();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	uint8 bAutoCalculateRotation : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(EditCondition="!bAutoCalculateRotation"))
	FRotator InitialRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	float Lifespan;

	/** The spawn distance among given direction. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	float SpawnDistance;

	/** The offset (local space) apply to calculated location based on direction and distance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	FVector SpawnOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	uint8 bAttachToOwner : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(EditCondition="bAttachToOwner"))
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(EditCondition="bAttachToOwner"))
	float AttachDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	uint8 bAutoDetach : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta=(EditCondition="bAutoDetach"))
	float AutoDetachDelay;

protected:

	UPROPERTY()
	AActor* SpawnedActor;

	USkeletalMeshComponent* OwnerComponent;

	FTimerHandle DelayAttachTimerHandle;
	FTimerHandle AutoDetachTimerHandle;
};

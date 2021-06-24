// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VfxBase.generated.h"

class UAudioComponent;

UCLASS(Blueprintable)
class DOGFIGHT_API AVfxBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVfxBase();

	virtual void SetTargetActor(AActor* Target);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Vfx")
	FString VfxName;

	/** The lifetime of this VFX. (0 means infinite.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Vfx")
	float Lifetime;

	/** The target socket when this vfx attach to actor. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Vfx")
	FName SocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Vfx")
	AController* OwnerController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Vfx Sound")
	TArray<USoundBase*> VfxSoundList;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Vfx")
	UAudioComponent* AudioComponent;
};

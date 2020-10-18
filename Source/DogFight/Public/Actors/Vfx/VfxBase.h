// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VfxBase.generated.h"

UCLASS(Blueprintable)
class DOGFIGHT_API AVfxBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVfxBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Vfx")
	FString VfxName;

	/** The lifetime of this VFX. (0 means infinite.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Vfx")
	float Lifetime;
};

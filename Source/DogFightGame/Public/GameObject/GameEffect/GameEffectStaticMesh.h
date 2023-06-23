// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameEffectBase.h"
#include "GameEffectStaticMesh.generated.h"

UCLASS(Blueprintable)
class DOGFIGHTGAME_API AGameEffectStaticMesh : public AGameEffectBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameEffectStaticMesh(const FObjectInitializer& ObjectInitializer);

	virtual void SetTarget(AActor* InTarget) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GameEffect")
	UStaticMeshComponent* MeshComponent;
};

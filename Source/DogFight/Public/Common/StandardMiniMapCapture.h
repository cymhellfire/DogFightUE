// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "StandardMiniMapCapture.generated.h"

class UTextureRenderTarget2D;

/**
 * 
 */
UCLASS(Blueprintable)
class DOGFIGHT_API AStandardMiniMapCapture : public ASceneCapture2D
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(Category=MiniMap, EditAnywhere, BlueprintReadWrite, meta=(ClampMin = "0", ClampMax = "1024"))
	float MiniMapHeight;

	UPROPERTY(Category=MiniMap, EditAnywhere, BlueprintReadWrite, meta=(ClampMin = "0", ClampMax = "1024"))
	float MiniMapWidth;

	/* Define the Z Axis offset for ground when capture Mini Map. */
	UPROPERTY(Category=MiniMap, EditAnywhere, BlueprintReadWrite)
	float GroundLevel;

#if WITH_EDITOR
	/* Filter out to apply delta Yaw */
	virtual void EditorApplyRotation(const FRotator& DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;

	/* Revert rotations other than roll */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	void UpdateWorldBounds();

	UPROPERTY()
	UTextureRenderTarget2D* MiniMapView;

	/* Last captured location */
	FVector CachedLocation;

	/* Last captured FOV */
	float CachedFOV;

	/* Last captured Ortho Width */
	float CachedOrthoWidth;

	/* Capture texture was resized */
	bool bTextureChanged;
};

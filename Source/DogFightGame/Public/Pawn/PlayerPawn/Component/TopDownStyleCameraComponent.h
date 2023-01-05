#pragma once

#include "Camera/CameraComponent.h"
#include "TopDownStyleCameraComponent.generated.h"

UCLASS()
class DOGFIGHTGAME_API UTopDownStyleCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	UTopDownStyleCameraComponent(const FObjectInitializer& ObjectInitializer);

	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;
	
	/**
	 * Clamp the camera location with world bounds.
	 *
	 * @param	PlayerController		The player controller relative to this camera.
	 * @param	OutLocation		Structure to receive the clamped coordinate.
	 */
	void ClampPawnLocation(const APlayerController* PlayerController, FVector& OutLocation);

protected:
	APlayerController* GetPlayerController() const;

	void UpdateViewBounds(const APlayerController* PlayerController);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	FRotator ViewAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float ViewDistance;

	/* Percentage of minimap where center of camera can be placed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Scroll)
	float MiniMapBoundsLimit;

protected:
	/* Bounds for camera movement. */
	FBox CameraMovementBounds;

	/* Viewport size associated with camera bounds. */
	FVector2D CameraMovementViewportSize;
};

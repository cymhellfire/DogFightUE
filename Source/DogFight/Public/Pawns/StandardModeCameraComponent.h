// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "StandardModeCameraComponent.generated.h"

UENUM()
enum class ECameraMovementType : uint8
{
	CMT_Normal,
	CMT_Dragged,
	CMT_FocusPoint,
};

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UStandardModeCameraComponent : public UCameraComponent
{
	GENERATED_UCLASS_BODY()

public:
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	void UpdateCameraMovement(const APlayerController* InPlayerController);

	void StartMoveToFocusPoint(FVector NewPoint);

	/* The angle of camera observes game scene. */
	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	FRotator CameraAngle;

	/* How far the camera from the ground. */
	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	float CameraDistance;

	/* Camera begin scroll when distance between cursor and screen edge smaller than this value. */
	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	float CameraActiveBorder;

	/* Scroll speed used by normal scroll method. */
	UPROPERTY(Category=Scroll, EditAnywhere, BlueprintReadWrite)
	float BaseScrollSpeed;

	/* Scroll speed used by dragging movement method. */
	UPROPERTY(Category=Scroll, EditAnywhere, BlueprintReadWrite)
	float MaxScrollSpeedDragging;

	/* Maximum scroll speed during dragging moving. */
	UPROPERTY(Category=Scroll, EditAnywhere, BlueprintReadWrite)
	float MaxSpeedDistanceWhenDragging;

	/* Percentage of minimap where center of camera can be placed. */
	UPROPERTY(Category=Scroll, EditAnywhere, BlueprintReadWrite)
	float MiniMapBoundsLimit;

	UPROPERTY(Category=Scroll, EditAnywhere, BlueprintReadWrite)
	float MoveToFocusThreshold;

	UPROPERTY(Category=Scroll, EditAnywhere, BlueprintReadWrite)
	float MoveToFocusDuration;

	/* Bounds for camera movement. */
	FBox CameraMovementBounds;

	/* Viewport size associated with camera bounds. */
	FVector2D CameraMovementViewportSize;

	UPROPERTY(Category=Scroll, EditAnywhere, BlueprintReadWrite)
	uint8 bShouldClampCamera : 1;

	/**
	 * Clamp the camera location with world bounds.
	 *
	 * @param	InPlayerController		The player controller relative to this camera.
	 * @param	OutCameraLocation		Structure to receive the clamped coordinate.
	 */
	void ClampCameraLocation(const APlayerController* InPlayerController, FVector& OutCameraLocation);

	/*
	 * Start mouse dragging movement for camera pawn.
	 */
	UFUNCTION(Category=Scroll, BlueprintCallable)
	void StartDraggingMovement();

	UFUNCTION(Category=Scroll, BlueprintCallable)
	void StopDraggingMovement();

private:

	ECameraMovementType GetCurrentMovementType() const
	{
		return CameraMovementTypeStack.Last();
	}

private:
	APlayerController* GetPlayerController() const;

	TArray<ECameraMovementType> CameraMovementTypeStack;

	/* Mouse position when dragging movement activated. */
	FVector2D MouseStartPosition;

	/* Update the movement bounds of this component. */
	void UpdateCameraBounds(const APlayerController* InPlayerController);

	float FocusMovingTimer;
	FVector FocusMovingStartPoint;
	FVector FocusMovingDeltaLoc;
};

// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "StandardModeCameraComponent.generated.h"

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

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	FRotator CameraAngle;

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	float CameraDistance;

	UPROPERTY(Category=Camera, EditAnywhere, BlueprintReadWrite)
	float CameraActiveBorder;

	/* Scroll speed used by normal scroll method. */
	UPROPERTY(Category=Scroll, EditAnywhere, BlueprintReadWrite)
	float BaseScrollSpeed;

	/* Scroll speed used by dragging movement method. */
	UPROPERTY(Category=Scroll, EditAnywhere, BlueprintReadWrite)
	float MaxScrollSpeedDragging;

	UPROPERTY(Category=Scroll, EditAnywhere, BlueprintReadWrite)
	float MaxSpeedDistanceWhenDragging;

	/*
	 * Start mouse dragging movement for camera pawn.
	 */
	UFUNCTION(Category=Scroll, BlueprintCallable)
	void StartDraggingMovement();

	UFUNCTION(Category=Scroll, BlueprintCallable)
	void StopDraggingMovement();

private:
	APlayerController* GetPlayerController() const;

	/* Return true if dragging movement is active currently. */
	bool bMouseDragging;

	/* Mouse position when dragging movement activated. */
	FVector2D MouseStartPosition;
};

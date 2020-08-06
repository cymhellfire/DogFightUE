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

private:
	APlayerController* GetPlayerController() const;
};

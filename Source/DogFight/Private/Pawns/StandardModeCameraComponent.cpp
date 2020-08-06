// Dog Fight Game Code By CYM.


#include "StandardModeCameraComponent.h"

#include "StandardModePlayerPawn.h"

UStandardModeCameraComponent::UStandardModeCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CameraAngle = FRotator(-55.f, 0.f, 0.f);
	CameraDistance = 1200;
	CameraActiveBorder = 20;
}

void UStandardModeCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	APlayerController* Controller = GetPlayerController();
	if (Controller != nullptr)
	{
		DesiredView.Location = Controller->GetFocalLocation() - CameraAngle.Vector() * CameraDistance;
		DesiredView.Rotation = CameraAngle;
	}
}

void UStandardModeCameraComponent::UpdateCameraMovement(const APlayerController* InPlayerController)
{
	// Only check movement if mouse available
#if PLATFORM_DESKTOP
	ULocalPlayer* const LocalPlayer = Cast<ULocalPlayer>(InPlayerController->GetLocalPlayer());
	if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->Viewport)
	{
		FVector2D MousePosition;
		if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition) == false)
		{
			return;
		}

		FViewport* Viewport = LocalPlayer->ViewportClient->Viewport;
		float const ScrollSpeed = 60.0f;
		const FIntPoint ViewportSize = Viewport->GetSizeXY();
		const uint32 ViewLeft = FMath::TruncToInt(LocalPlayer->Origin.X * ViewportSize.X);
		const uint32 ViewRight = ViewLeft + FMath::TruncToInt(LocalPlayer->Size.X * ViewportSize.X);
		const uint32 ViewTop = FMath::TruncToInt(LocalPlayer->Origin.X * ViewportSize.Y);
		const uint32 ViewBottom = ViewTop + FMath::TruncToInt(LocalPlayer->Size.Y * ViewportSize.Y);

		const uint32 MouseX = MousePosition.X;
		const uint32 MouseY = MousePosition.Y;
		AStandardModePlayerPawn* PlayerPawn = Cast<AStandardModePlayerPawn>(GetOwner());

		if (MouseX >= ViewLeft && MouseX <= (ViewLeft + CameraActiveBorder))
		{
			const float Delta = 1.0f - float(MouseX - ViewLeft) / CameraActiveBorder;
			PlayerPawn->MoveRight(-ScrollSpeed * Delta);
		}
		else if (MouseX >= (ViewRight - CameraActiveBorder) && MouseX <= ViewRight)
		{
			const float Delta = float(ViewRight - MouseX) / CameraActiveBorder;
			PlayerPawn->MoveRight(ScrollSpeed * Delta);
		}

		if (MouseY >= ViewTop && MouseY <= (ViewTop + CameraActiveBorder))
		{
			const float Delta = 1.0f - float(MouseY - ViewTop) / CameraActiveBorder;
			PlayerPawn->MoveForward(ScrollSpeed * Delta);
		}
		else if (MouseY >= (ViewBottom - CameraActiveBorder) && MouseY <= ViewBottom)
		{
			const float Delta = float(MouseY - (ViewBottom - CameraActiveBorder)) / CameraActiveBorder;
			PlayerPawn->MoveForward(-ScrollSpeed * Delta);
		}
	}
#endif
}

APlayerController* UStandardModeCameraComponent::GetPlayerController() const
{
	APlayerController* Controller = nullptr;
	APawn* Owner = Cast<APawn>(GetOwner());
	if (Owner != nullptr)
	{
		Controller = Cast<APlayerController>(Owner->GetController());
	}
	return Controller;
}

// Dog Fight Game Code By CYM.


#include "StandardModeCameraComponent.h"

#include "StandardModePlayerPawn.h"
#include "DogFight/DogFight.h"

UStandardModeCameraComponent::UStandardModeCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	bMouseDragging(false)
{
	CameraAngle = FRotator(-55.f, 0.f, 0.f);
	CameraDistance = 1200;
	CameraActiveBorder = 20;
	BaseScrollSpeed = 50.f;
	MaxScrollSpeedDragging = 60.f;
	MaxSpeedDistanceWhenDragging = 100.f;
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

		AStandardModePlayerPawn* PlayerPawn = Cast<AStandardModePlayerPawn>(GetOwner());
		float const DeltaTime = GetWorld()->GetDeltaSeconds();
		// Check if mouse dragging movement is active
		if (bMouseDragging)
		{
			const FVector2D MousePosDelta = MousePosition - MouseStartPosition;
			const float MoveX = MousePosDelta.X / MaxSpeedDistanceWhenDragging * MaxScrollSpeedDragging * DeltaTime;
			const float MoveY = MousePosDelta.Y / MaxSpeedDistanceWhenDragging * -MaxScrollSpeedDragging * DeltaTime;
			PlayerPawn->MoveRight(MoveX);
			PlayerPawn->MoveForward(MoveY);
		}
		else
		{
			FViewport* Viewport = LocalPlayer->ViewportClient->Viewport;
			const FIntPoint ViewportSize = Viewport->GetSizeXY();
			const uint32 ViewLeft = FMath::TruncToInt(LocalPlayer->Origin.X * ViewportSize.X);
			const uint32 ViewRight = ViewLeft + FMath::TruncToInt(LocalPlayer->Size.X * ViewportSize.X);
			const uint32 ViewTop = FMath::TruncToInt(LocalPlayer->Origin.X * ViewportSize.Y);
			const uint32 ViewBottom = ViewTop + FMath::TruncToInt(LocalPlayer->Size.Y * ViewportSize.Y);

			const uint32 MouseX = MousePosition.X;
			const uint32 MouseY = MousePosition.Y;

			const float DeltaScrollSpeed = BaseScrollSpeed * DeltaTime;

			if (MouseX >= ViewLeft && MouseX <= (ViewLeft + CameraActiveBorder))
			{
				const float Delta = 1.0f - float(MouseX - ViewLeft) / CameraActiveBorder;
				UE_LOG(LogDogFight, Display, TEXT("Current Delta = %f"), Delta);
				PlayerPawn->MoveRight(-DeltaScrollSpeed * Delta);
			}
			else if (MouseX >= (ViewRight - CameraActiveBorder) && MouseX <= ViewRight)
			{
				const float Delta = float(MouseX - (ViewRight - CameraActiveBorder)) / CameraActiveBorder;
				UE_LOG(LogDogFight, Display, TEXT("Current Delta = %f"), Delta);
				PlayerPawn->MoveRight(DeltaScrollSpeed * Delta);
			}

			if (MouseY >= ViewTop && MouseY <= (ViewTop + CameraActiveBorder))
			{
				const float Delta = 1.0f - float(MouseY - ViewTop) / CameraActiveBorder;
				PlayerPawn->MoveForward(DeltaScrollSpeed * Delta);
			}
			else if (MouseY >= (ViewBottom - CameraActiveBorder) && MouseY <= ViewBottom)
			{
				const float Delta = float(MouseY - (ViewBottom - CameraActiveBorder)) / CameraActiveBorder;
				PlayerPawn->MoveForward(-DeltaScrollSpeed * Delta);
			}
		}
	}
#endif
}

void UStandardModeCameraComponent::StartDraggingMovement()
{
	bMouseDragging = true;

	// Record current mouse position on screen
	ULocalPlayer* const LocalPlayer = Cast<ULocalPlayer>(GetPlayerController()->GetLocalPlayer());
	if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->Viewport)
	{
		if (!LocalPlayer->ViewportClient->GetMousePosition(MouseStartPosition))
		{
			UE_LOG(LogDogFight, Error, TEXT("No mouse position available."));
		}
	}
}

void UStandardModeCameraComponent::StopDraggingMovement()
{
	bMouseDragging = false;
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

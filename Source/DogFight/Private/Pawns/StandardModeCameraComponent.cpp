// Dog Fight Game Code By CYM.


#include "Pawns/StandardModeCameraComponent.h"

#include "MathHelper.h"
#include "Game/StandardGameState.h"
#include "Pawns/StandardModePlayerPawn.h"
#include "DogFight/DogFight.h"

UStandardModeCameraComponent::UStandardModeCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CameraAngle = FRotator(-55.f, 0.f, 0.f);
	CameraDistance = 1200;
	CameraActiveBorder = 20;
	BaseScrollSpeed = 50.f;
	MaxScrollSpeedDragging = 60.f;
	MaxSpeedDistanceWhenDragging = 100.f;
	MiniMapBoundsLimit = 0.8f;
	bShouldClampCamera = 1;
	MoveToFocusThreshold = 1.f;
	MoveToFocusDuration = 1.f;
	CameraMovementTypeStack.Add(ECameraMovementType::CMT_Normal);
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
		if (GetCurrentMovementType() == ECameraMovementType::CMT_Dragged)
		{
			const FVector2D MousePosDelta = MousePosition - MouseStartPosition;
			const float MoveX = MousePosDelta.X / MaxSpeedDistanceWhenDragging * MaxScrollSpeedDragging * DeltaTime;
			const float MoveY = MousePosDelta.Y / MaxSpeedDistanceWhenDragging * -MaxScrollSpeedDragging * DeltaTime;
			PlayerPawn->MoveRight(MoveX);
			PlayerPawn->MoveForward(MoveY);
		}
		if (GetCurrentMovementType() == ECameraMovementType::CMT_Normal)
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
				PlayerPawn->MoveRight(-DeltaScrollSpeed * Delta);
			}
			else if (MouseX >= (ViewRight - CameraActiveBorder) && MouseX <= ViewRight)
			{
				const float Delta = float(MouseX - (ViewRight - CameraActiveBorder)) / CameraActiveBorder;
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

	// Move to focus point
	if (GetCurrentMovementType() == ECameraMovementType::CMT_FocusPoint)
	{
		AStandardModePlayerPawn* PlayerPawn = Cast<AStandardModePlayerPawn>(GetOwner());

		const float DeltaTime = GetWorld()->GetDeltaSeconds();
		FocusMovingTimer += DeltaTime;

		const FVector NewLoc = FocusMovingStartPoint + (FMath::Clamp<float>(FocusMovingTimer, 0.f, MoveToFocusDuration) / MoveToFocusDuration) * FocusMovingDeltaLoc;
		PlayerPawn->SetActorLocation(NewLoc);

		if (FocusMovingTimer >= MoveToFocusDuration)
		{
			CameraMovementTypeStack.RemoveSingle(ECameraMovementType::CMT_FocusPoint);
		}
	}
}

void UStandardModeCameraComponent::StartMoveToFocusPoint(FVector NewPoint)
{
	FocusMovingStartPoint = GetOwner()->GetActorLocation();
	// Filter out unnecessary movement
	if ((NewPoint - FocusMovingStartPoint).Size() <= MoveToFocusThreshold)
	{
		return;
	}

	FocusMovingDeltaLoc = NewPoint - FocusMovingStartPoint;
	FocusMovingTimer = 0.f;
	CameraMovementTypeStack.AddUnique(ECameraMovementType::CMT_FocusPoint);
}

void UStandardModeCameraComponent::ClampCameraLocation(const APlayerController* InPlayerController,
                                                       FVector& OutCameraLocation)
{
	if (bShouldClampCamera)
	{
		UpdateCameraBounds(InPlayerController);
		if (CameraMovementBounds.GetSize() != FVector::ZeroVector)
		{
			OutCameraLocation = CameraMovementBounds.GetClosestPointTo(OutCameraLocation);
		}
	}
}

void UStandardModeCameraComponent::StartDraggingMovement()
{
	CameraMovementTypeStack.Add(ECameraMovementType::CMT_Dragged);

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
	CameraMovementTypeStack.RemoveSingle(ECameraMovementType::CMT_Dragged);
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



void UStandardModeCameraComponent::UpdateCameraBounds(const APlayerController* InPlayerController)
{
	ULocalPlayer* const LocalPlayer = Cast<ULocalPlayer>(InPlayerController->Player);
	if (LocalPlayer == nullptr || LocalPlayer->ViewportClient == nullptr)
	{
		return;
	}

	FVector2D CurrentViewportSize;
	LocalPlayer->ViewportClient->GetViewportSize(CurrentViewportSize);

	// Calculate frustum edge direction, from bottom left corner
	if (CameraMovementBounds.GetSize() == FVector::ZeroVector || CurrentViewportSize != CameraMovementViewportSize)
	{
		const FVector FrustumRay2DDir = GetComponentRotation().Vector();
		const FVector FrustumRay2DRight = FVector::CrossProduct(FrustumRay2DDir, FVector::UpVector);
		const FQuat RotQuat(FrustumRay2DRight, FMath::DegreesToRadians(90.f - InPlayerController->PlayerCameraManager->GetFOVAngle() * 0.5f));
		const FVector FrustumRayDir = RotQuat.RotateVector(FrustumRay2DDir);

		// Collect 3 world bounds' points and matching frustum rays (bottom left, top left, bottom right)
		AStandardGameState const* const MyGameState = GetWorld()->GetGameState<AStandardGameState>();
		if (MyGameState != nullptr)
		{
			FBox const& WorldBounds = MyGameState->WorldBounds;
			if (WorldBounds.GetSize() != FVector::ZeroVector)
			{
				const FVector WorldBoundPoints[] = {
					FVector(WorldBounds.Min.X, WorldBounds.Min.Y, WorldBounds.Max.Z),
					FVector(WorldBounds.Min.X, WorldBounds.Max.Y, WorldBounds.Max.Z),
					FVector(WorldBounds.Max.X, WorldBounds.Min.Y, WorldBounds.Max.Z)
				};
				const FVector FrustumRays[] = {
					FVector(FrustumRayDir.X, FrustumRayDir.Y, FrustumRayDir.Z),
					FVector(FrustumRayDir.X, -FrustumRayDir.Y, FrustumRayDir.Z),
					FVector(-FrustumRayDir.X, FrustumRayDir.Y, FrustumRayDir.Z)
				};

				// Get camera plane for intersection
				const FPlane CameraPlane = FPlane(InPlayerController->GetFocalLocation(), FVector::UpVector);

				const FVector CameraPlanePoints[3] = {
					FMathHelper::IntersectRayWithPlane(WorldBoundPoints[0], FrustumRays[0], CameraPlane) * MiniMapBoundsLimit,
					FMathHelper::IntersectRayWithPlane(WorldBoundPoints[1], FrustumRays[1], CameraPlane) * MiniMapBoundsLimit,
					FMathHelper::IntersectRayWithPlane(WorldBoundPoints[2], FrustumRays[2], CameraPlane) * MiniMapBoundsLimit
				};

				// create new bounds
				CameraMovementBounds = FBox(CameraPlanePoints, 3);
				CameraMovementViewportSize = CurrentViewportSize;
			}
		}
	}
}

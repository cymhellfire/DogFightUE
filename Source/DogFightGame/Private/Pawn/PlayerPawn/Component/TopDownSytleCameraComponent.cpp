#include "GameMode/TopDownStyleGameState.h"
#include "Math/MathHelper.h"
#include "Pawn/PlayerPawn/Component/TopDownStyleCameraComponent.h"

UTopDownStyleCameraComponent::UTopDownStyleCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ViewAngle = FRotator(-55.f, 0.f, 0.f);
	ViewDistance = 1200;
	MiniMapBoundsLimit = 0.8f;
}

void UTopDownStyleCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	if (APlayerController* PlayerController = GetPlayerController())
	{
		DesiredView.Location = PlayerController->GetFocalLocation() - ViewAngle.Vector() * ViewDistance;
		DesiredView.Rotation = ViewAngle;
	}
	else
	{
		Super::GetCameraView(DeltaTime, DesiredView);
	}
}

void UTopDownStyleCameraComponent::ClampPawnLocation(const APlayerController* PlayerController, FVector& OutLocation)
{
	UpdateViewBounds(PlayerController);
	if (CameraMovementBounds.GetSize() != FVector::ZeroVector)
	{
		OutLocation = CameraMovementBounds.GetClosestPointTo(OutLocation);
	}
}

APlayerController* UTopDownStyleCameraComponent::GetPlayerController() const
{
	APlayerController* Controller = nullptr;
	if (APawn* Owner = Cast<APawn>(GetOwner()))
	{
		Controller = Cast<APlayerController>(Owner->GetController());
	}
	return Controller;
}

void UTopDownStyleCameraComponent::UpdateViewBounds(const APlayerController* PlayerController)
{
	ULocalPlayer* const LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);
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
		const FQuat RotQuat(FrustumRay2DRight, FMath::DegreesToRadians(90.f - PlayerController->PlayerCameraManager->GetFOVAngle() * 0.5f));
		const FVector FrustumRayDir = RotQuat.RotateVector(FrustumRay2DDir);

		// Collect 3 world bounds' points and matching frustum rays (bottom left, top left, bottom right)
		ATopDownStyleGameState const* const MyGameState = GetWorld()->GetGameState<ATopDownStyleGameState>();
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
				const FPlane CameraPlane = FPlane(PlayerController->GetFocalLocation(), FVector::UpVector);

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


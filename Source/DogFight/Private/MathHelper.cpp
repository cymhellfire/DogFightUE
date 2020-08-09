// Dog Fight Game Code By CYM.


#include "MathHelper.h"

#include "DogFight/DogFight.h"

bool FMathHelper::DeprojectScreenToWorld(const FVector2D& ScreenPosition, ULocalPlayer* Player, FVector& RayOrigin,
	FVector& RayDirection)
{
	if (Player != nullptr && Player->ViewportClient != nullptr && Player->ViewportClient->Viewport != nullptr 
		&& Player->PlayerController != nullptr)
	{
		FSceneViewProjectionData ProjectionData;
		if (Player->GetProjectionData(Player->ViewportClient->Viewport, eSSP_FULL, ProjectionData))
		{
			const FMatrix ViewMatrix = FTranslationMatrix(-ProjectionData.ViewOrigin) * ProjectionData.ViewRotationMatrix;
			const FMatrix InvViewMatrix = ViewMatrix.InverseFast();
			const FMatrix InvProjectionMatrix = ProjectionData.ProjectionMatrix.InverseFast();

			FSceneView::DeprojectScreenToWorld(ScreenPosition, ProjectionData.GetConstrainedViewRect(), InvViewMatrix,
			                                   InvProjectionMatrix, RayOrigin, RayDirection);

			return true;
		}
	}

	return false;
}

FVector FMathHelper::IntersectRayWithPlane(const FVector& RayOrigin, const FVector& RayDirection, const FPlane& Plane)
{
	const FVector PlaneNormal = FVector(Plane.X, Plane.Y, Plane.Z);
	const FVector PlaneOrigin = PlaneNormal * Plane.W;

	const float Distance = FVector::DotProduct((PlaneOrigin - RayOrigin), PlaneNormal) / FVector::DotProduct(RayDirection, PlaneNormal);
	return RayOrigin + RayDirection * Distance;
}

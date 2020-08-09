// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DOGFIGHT_API FMathHelper
{
public:
	/* Convert point in screen space to ray in world space. */
	static bool DeprojectScreenToWorld(const FVector2D& ScreenPosition, class ULocalPlayer* Player, FVector& RayOrigin, FVector& RayDirection);

	/* Find intersection of ray with specified plane in world space. */
	static FVector IntersectRayWithPlane(const FVector& RayOrigin, const FVector& RayDirection, const FPlane& Plane);
};

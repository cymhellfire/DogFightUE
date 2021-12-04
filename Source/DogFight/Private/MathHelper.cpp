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
		if (Player->GetProjectionData(Player->ViewportClient->Viewport, ProjectionData,
			GEngine->IsStereoscopic3D() ? EStereoscopicEye::eSSE_LEFT_EYE : EStereoscopicEye::eSSE_MONOSCOPIC))
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

int32 FMathHelper::MinExcludeNumber = 0;

int32 FMathHelper::GetMissingNumber(TArray<int32> InArray, int32 MinNumber = 0)
{
	if (MinNumber != 0)
	{
		MinExcludeNumber = MinNumber;
	}
	
	const int32 ArraySize = InArray.Num();
	const int32 Result = FindMissing(InArray, ArraySize);

	// Restore the minimum exclude number
	if (MinExcludeNumber != 0)
	{
		MinExcludeNumber = 0;
	}

	return Result;
}

int32 FMathHelper::FindMissing(TArray<int32> InArray, int32 Size)
{
	// First separate positive and negative numbers
	const int32 Shift = Segregate(InArray, Size);
	TArray<int32> NewArray;

	for (int i = Shift; i < Size; ++i)
	{
		NewArray.Add(InArray[i] - MinExcludeNumber);
	}

	return FindMissingPositive(NewArray, NewArray.Num()) + MinExcludeNumber;
}

int32 FMathHelper::FindMissingPositive(TArray<int32> InArray, int32 Size)
{
	int32 i;

	/**
	 * Mark InArray[i] as visited by making InArray[InArray[i] - 1] negative.
	 * Note that 1 is subtracted as index starts from 0 and positive numbers
	 * starts from 1.
	 */
	for (i = 0; i < Size; ++i)
	{
		if (FMath::Abs(InArray[i]) - 1 < Size && InArray[FMath::Abs(InArray[i]) - 1] > 0)
		{
			InArray[FMath::Abs(InArray[i]) - 1] = -InArray[FMath::Abs(InArray[i]) - 1];
		}
	}

	/**
	 * Return the first index value at which is positive
	 */
	for (i = 0; i < Size; ++i)
	{
		if (InArray[i] > 0)
			return i + 1;
	}

	// 1 is added because indexes start from 0
	return Size + 1;
}

int32 FMathHelper::Segregate(TArray<int32> InArray, int32 Size)
{
	int32 j = 0;
	for (int32 i = 0; i < Size; ++i)
	{
		if (InArray[i] <= MinExcludeNumber)
		{
			const int32 Temp = InArray[i];
			InArray[i] = InArray[j];
			InArray[j] = Temp;

			// Increase count of non-positive number
			j++;
		}
	}

	return j;
}

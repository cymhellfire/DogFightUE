// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROJECTFRAMEWORK_API FMathHelper
{
public:
	/* Convert point in screen space to ray in world space. */
	static bool DeprojectScreenToWorld(const FVector2D& ScreenPosition, class ULocalPlayer* Player, FVector& RayOrigin, FVector& RayDirection);

	/* Find intersection of ray with specified plane in world space. */
	static FVector IntersectRayWithPlane(const FVector& RayOrigin, const FVector& RayDirection, const FPlane& Plane);

	/**
	* Find the smallest positive missing number in array that contains both
	* positive and negative integers.
	*/
	static int32 GetMissingNumber(TArray<int32> InArray, int32 MinNumber);

private:
	/** The minimum number that excluded from finding missing number. */
	static int32 MinExcludeNumber;

	/**
	* Find the smallest positive missing number in array that contains both
	* positive and negative integers.
	*
	* @param InArray	Array to check missing number
	* @param Size		Array length
	*/
	static int32 FindMissing(TArray<int32> InArray, int32 Size);

	/**
	* Find the smallest positive missing number in an array that contains
	* all positive integers.
	*/
	static int32 FindMissingPositive(TArray<int32> InArray, int32 Size);

	/**
	* Utility function that puts all non-positive (0 and negative) numbers
	* on left side of InArray and return count of such numbers.
	*/
	static int32 Segregate(TArray<int32> InArray, int32 Size);
};

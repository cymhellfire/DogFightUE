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

	/**
	 * @brief Randomize the array order with Fisher-Yates Shuffle algorithm
	 * @tparam T Element data type.
	 * @param InArray Array to randomize order.
	 */
	template<typename T>
	static void RandomizeArray(TArray<T>& InArray)
	{
		const int32 Length = InArray.Num();
		if (Length <= 1)
		{
			return;
		}

		for (int32 i = Length - 1; i > 0; --i)
		{
			T Temp = InArray[i];
			const int32 SwitchTargetIndex = FMath::RandRange(0, i);
			// Switch value
			InArray[i] = InArray[SwitchTargetIndex];
			InArray[SwitchTargetIndex] = Temp;
		}
	}

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

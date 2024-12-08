// Dog Fight Game Code By CYM.


#include "FunctionLibrary/MathHelperLibrary.h"
#include "Math/MathHelper.h"

int32 UMathHelperLibrary::GetMinMissingPositiveNumber(TArray<int32> InArray, int32 MinNumber)
{
	return FMathHelper::GetMissingNumber(InArray, MinNumber);
}

TArray<int32> UMathHelperLibrary::RandomizedOrder(const TArray<int32>& IndexArray)
{
	auto Result = IndexArray;
	FMathHelper::RandomizeArray(Result);
	return Result;
}

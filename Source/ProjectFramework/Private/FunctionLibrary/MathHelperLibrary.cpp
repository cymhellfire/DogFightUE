// Dog Fight Game Code By CYM.


#include "FunctionLibrary/MathHelperLibrary.h"
#include "Math/MathHelper.h"

int32 UMathHelperLibrary::GetMinMissingPositiveNumber(TArray<int32> InArray, int32 MinNumber)
{
	return FMathHelper::GetMissingNumber(InArray, MinNumber);
}
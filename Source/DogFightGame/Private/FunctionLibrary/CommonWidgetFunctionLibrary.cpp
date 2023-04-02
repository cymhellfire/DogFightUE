#include "FunctionLibrary/CommonWidgetFucntionLibrary.h"

FSlateColor UCommonWidgetFunctionLibrary::MakeSlateColor(float R, float G, float B, float A)
{
	return FSlateColor(FLinearColor{R, G, B, A});
}


#pragma once

namespace ETokenType
{
	inline FString TokenTypeName[] =
	{
		TEXT("None"),
		TEXT("Reserved"),
		TEXT("Unary Operator"),
		TEXT("Binary Operator"),
		TEXT("ID"),
		TEXT("Value"),
	};
}

namespace EReservedType
{
	inline FString ReservedTypeName[] =
	{
		TEXT("None"),
		TEXT("If"),
		TEXT("Else"),
		TEXT("For"),
		TEXT("While"),
		TEXT("Break"),
		TEXT("Continue"),
		TEXT("Class"),
		TEXT("Function"),
	};
}

namespace EUnaryOperatorType
{
	inline FString UnaryOperatorTypeName[] =
	{
		TEXT("None"),
		TEXT("Negative"),
		TEXT("Not"),
	};
}

namespace EBinaryOperatorType
{
	inline FString BinaryOperatorTypeName[] =
	{
		TEXT("None"),
		TEXT("Add"),
		TEXT("Subtract"),
		TEXT("Multiply"),
		TEXT("Divide"),
		TEXT("Mod"),
		TEXT("Factorial"),
		TEXT("Assign"),
		TEXT("Equal"),
		TEXT("Larger"),
		TEXT("LargerEqual"),
		TEXT("Less"),
		TEXT("LessEqual"),
	};
}

namespace EValueType
{
	inline FString ValueTypeName[] =
	{
		TEXT("None"),
		TEXT("Number"),
		TEXT("String"),
	};
}

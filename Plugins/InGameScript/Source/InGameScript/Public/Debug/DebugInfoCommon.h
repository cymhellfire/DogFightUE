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
		TEXT("Single Symbol"),
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
		TEXT("Return"),
		TEXT("Number"),
		TEXT("String"),
		TEXT("Boolean"),
		TEXT("Void"),
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

namespace ESingleSymbolType
{
	inline FString SingleSymbolTypeName[] =
	{
		TEXT("None"),
		TEXT("Period"),
		TEXT("OpenParen"),
		TEXT("CloseParen"),
		TEXT("OpenBracket"),
		TEXT("CloseBracket"),
		TEXT("OpenCurly"),
		TEXT("CloseCurly"),
		TEXT("Semicolon"),
		TEXT("Comma"),
	};
}

namespace EValueType
{
	inline FString ValueTypeName[] =
	{
		TEXT("None"),
		TEXT("Boolean"),
		TEXT("Number"),
		TEXT("String"),
		TEXT("Void"),
		TEXT("Class"),
	};
}

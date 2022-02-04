#pragma once

#if WITH_DEBUG_INFO
#include "Debug/DebugInfoCommon.h"
#endif

#define INVALID_CHAR		-1

namespace EASTNodeType
{
	enum Type
	{
		ANT_None,
		ANT_Expression,
		ANT_Value,
	};
};

namespace ETokenType
{
	enum Type
	{
		TT_None,
		TT_Reserved,
		TT_UnaryOperator,
		TT_BinaryOperator,
		TT_ID,
		TT_Value,
	};
}

namespace EReservedType
{
	enum Type
	{
		RT_None,
		RT_If,
		RT_Else,
		RT_For,
		RT_While,
		RT_Break,
		RT_Continue,
		RT_Class,
		RT_Function,
	};
}

namespace EUnaryOperatorType
{
	enum Type
	{
		UOT_None,
		UOT_Negative,
		UOT_Not,
	};
}

namespace EBinaryOperatorType
{
	enum Type
	{
		BOT_None,
		BOT_Add,
		BOT_Subtract,
		BOT_Multiply,
		BOT_Divide,
		BOT_Mod,
		BOT_Factorial,
		BOT_Assign,
		BOT_Equal,
		BOT_Larger,
		BOT_LargerEqual,
		BOT_Less,
		BOT_LessEqual,
	};
}

namespace EValueType
{
	enum Type
	{
		VT_None,
		VT_Number,
		VT_String,
	};
}

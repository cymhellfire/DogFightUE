#pragma once

#if WITH_DEBUG_INFO
#include "Debug/DebugInfoCommon.h"
#endif

#define INVALID_CHAR		-1

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
		TT_SingleSymbol,
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
		RT_Return,
		RT_Number,
		RT_String,
		RT_Boolean,
		RT_Void,
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

	static int32 PriorityTable[] =
	{
		0, // BOT_None,
		2, // BOT_Add,
		2, // BOT_Subtract,
		3, // BOT_Multiply,
		3, // BOT_Divide,
		3, // BOT_Mod,
		4, // BOT_Factorial,
		1, // BOT_Assign,
		2, // BOT_Equal,
		2, // BOT_Larger,
		2, // BOT_LargerEqual,
		2, // BOT_Less,
		2, // BOT_LessEqual,
	};

	static int32 MaxPriority = 4;
	static int32 MinPriority = 1;
}

namespace ESingleSymbolType
{
	enum Type
	{
		SST_None,
		SST_Period,
		SST_OpenParen,
		SST_CloseParen,
		SST_OpenBracket,
		SST_CloseBracket,
		SST_OpenCurly,
		SST_CloseCurly,
		SST_Semicolon,
		SST_Comma,
	};
}

namespace EValueType
{
	enum Type
	{
		VT_None,
		VT_Boolean,
		VT_Number,
		VT_String,
	};
}

namespace EASTNodeType
{
	enum Type
	{
		ANT_None,
		ANT_Block,
		ANT_Statement,
		ANT_Function,
		ANT_Class,
		ANT_If,
		ANT_For,
		ANT_BinaryOp,
		ANT_UnaryOp,
		ANT_SimpleExpression,
		ANT_PrefixExpression,
		ANT_SuffixExpression,
		ANT_Value,
		ANT_Assign,
		ANT_Return,
		ANT_FunctionInvoker,
		ANT_MemberAccessor,
		ANT_IndexAccessor,
	};
}

#pragma once

#include "Common.h"

struct FTokenBase
{
	virtual ~FTokenBase() {}

#if WITH_DEBUG_INFO
	virtual FString ToString() const
	{
		return FString::Printf(TEXT("%s Token"), *ETokenType::TokenTypeName[TokenType]);
	}
#endif

	ETokenType::Type TokenType;
};

struct FReservedToken : FTokenBase
{
	FReservedToken()
		: FReservedToken(EReservedType::RT_None)
	{
	}

	FReservedToken(EReservedType::Type InType)
	{
		TokenType = ETokenType::TT_Reserved;
		ReservedType = InType;
	}

#if WITH_DEBUG_INFO
	virtual FString ToString() const override
	{
		return FString::Printf(TEXT("%s: %s"), *FTokenBase::ToString(), *EReservedType::ReservedTypeName[ReservedType]);
	}
#endif

	EReservedType::Type ReservedType;
};

struct FOperatorToken : FTokenBase
{
	FOperatorToken()
		: FOperatorToken(EUnaryOperatorType::UOT_None)
	{
	}

	FOperatorToken(EUnaryOperatorType::Type InType)
	{
		TokenType = ETokenType::TT_UnaryOperator;
		UnaryOperatorType = InType;
		BinaryOperatorType = EBinaryOperatorType::BOT_None;
	}

	FOperatorToken(EBinaryOperatorType::Type InType)
	{
		TokenType = ETokenType::TT_BinaryOperator;
		UnaryOperatorType = EUnaryOperatorType::UOT_None;
		BinaryOperatorType = InType;
	}

#if WITH_DEBUG_INFO
	virtual FString ToString() const override
	{
		if (TokenType == ETokenType::TT_UnaryOperator)
		{
			return FString::Printf(TEXT("%s: %s"), *FTokenBase::ToString(), *EUnaryOperatorType::UnaryOperatorTypeName[UnaryOperatorType]);
		}
		else
		{
			return FString::Printf(TEXT("%s: %s"), *FTokenBase::ToString(), *EBinaryOperatorType::BinaryOperatorTypeName[BinaryOperatorType]);
		}
	}
#endif

	EUnaryOperatorType::Type UnaryOperatorType;
	EBinaryOperatorType::Type BinaryOperatorType;
};

struct FIDToken : FTokenBase
{
	FIDToken()
		: FIDToken("")
	{
	}

	FIDToken(FString InValue)
	{
		IdName = InValue;
		TokenType = ETokenType::TT_ID;
	}

#if WITH_DEBUG_INFO
	virtual FString ToString() const override
	{
		return FString::Printf(TEXT("%s: %s"), *FTokenBase::ToString(), *IdName);
	}
#endif

	FString IdName;
};

struct FValueToken : FTokenBase
{
	FValueToken()
		: FValueToken(0.f)
	{
	}

	FValueToken(float InValue)
	{
		TokenType = ETokenType::TT_Value;
		ValueType = EValueType::VT_Number;
		NumberValue = InValue;
	}

	FValueToken(FString InValue)
	{
		TokenType = ETokenType::TT_Value;
		ValueType = EValueType::VT_String;
		StringValue = InValue;
	}

#if WITH_DEBUG_INFO
	virtual FString ToString() const override
	{
		if (ValueType == EValueType::VT_Number)
		{
			return FString::Printf(TEXT("%s: %s[%.3f]"), *FTokenBase::ToString(), *EValueType::ValueTypeName[ValueType],
				NumberValue);
		}
		else
		{
			return FString::Printf(TEXT("%s: %s[%s]"), *FTokenBase::ToString(), *EValueType::ValueTypeName[ValueType],
				*StringValue);
		}
	}
#endif

	EValueType::Type ValueType;

	float NumberValue;
	FString StringValue;
};

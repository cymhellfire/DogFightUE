#pragma once

/**
 * @brief Make a boolean type attribute inner struct for given class member.
 * @param Class			The class holding attribute wrapper member and callback function.
 * @param Member		The member which new attribute created for.
 * @param Callback		The callback function triggered when inner attribute value changed.
 */
#define MAKE_BOOL_ATTRIBUTE_FOR_MEMBER(Class, Member, Callback)	\
	const FName NewAttrName = GET_MEMBER_NAME_CHECKED(Class, Member); \
	FAttributeCreateArgument NewAttributeArgument; \
	NewAttributeArgument.AttrName = NewAttrName; \
	NewAttributeArgument.DataType = ADT_Boolean; \
	NewAttributeArgument.InitBooleanValue = Member.BaseValue; \
	if (AddAttribute(NewAttributeArgument)) \
	{ \
		auto NewAttribute = GetAttribute(NewAttrName); \
		NewAttribute->OnValueChanged.AddUObject(this, &Class::Callback); \
	}

/**
 * @brief Make a integer type attribute inner struct for given class member.
 * @param Class			The class holding attribute wrapper member and callback function.
 * @param Member		The member which new attribute created for.
 * @param Callback		The callback function triggered when inner attribute value changed.
 */
#define MAKE_INT_ATTRIBUTE_FOR_MEMBER(Class, Member, Callback)	\
	const FName NewAttrName = GET_MEMBER_NAME_CHECKED(Class, Member); \
	FAttributeCreateArgument NewAttributeArgument; \
	NewAttributeArgument.AttrName = NewAttrName; \
	NewAttributeArgument.DataType = ADT_Integer; \
	NewAttributeArgument.InitIntegerValue = Member.BaseValue; \
	if (AddAttribute(NewAttributeArgument)) \
	{ \
		auto NewAttribute = GetAttribute(NewAttrName); \
		NewAttribute->OnValueChanged.AddUObject(this, &Class::Callback); \
	}

/**
 * @brief Make a float type attribute inner struct for given class member.
 * @param Class			The class holding attribute wrapper member and callback function.
 * @param Member		The member which new attribute created for.
 * @param Callback		The callback function triggered when inner attribute value changed.
 */
#define MAKE_FLOAT_ATTRIBUTE_FOR_MEMBER(Class, Member, Callback)	\
	const FName NewAttrName = GET_MEMBER_NAME_CHECKED(Class, Member); \
	FAttributeCreateArgument NewAttributeArgument; \
	NewAttributeArgument.AttrName = NewAttrName; \
	NewAttributeArgument.DataType = ADT_Float; \
	NewAttributeArgument.InitFloatValue = Member.BaseValue; \
	if (AddAttribute(NewAttributeArgument)) \
	{ \
		auto NewAttribute = GetAttribute(NewAttrName); \
		NewAttribute->OnValueChanged.AddUObject(this, &Class::Callback); \
	}


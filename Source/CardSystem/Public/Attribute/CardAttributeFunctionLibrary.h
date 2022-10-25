#pragma once

#include "CoreMinimal.h"

class UCard;
template <typename T>
class TAttributeBase;

class CARDSYSTEM_API FCardAttributeFunctionLibrary
{
public:
	static bool CreateBoolAttributeForCard(UCard* InCard, FName InName, bool InValue);

	static bool CreateIntegerAttributeForCard(UCard* InCard, FName InName, int32 InValue);

	static bool CreateFloatAttributeForCard(UCard* InCard, FName InName, float InValue);

	static bool GetBoolAttributeValueFromCard(UCard* InCard, FName InName, bool& OutValue);

	static bool GetIntegerAttributeValueFromCard(UCard* InCard, FName InName, int32& OutValue);

	static bool GetFloatAttributeValueFromCard(UCard* InCard, FName InName, float& OutValue);

protected:
	template<typename T>
	static bool AddAttributeToCard(UCard* InCard, TSharedPtr<TAttributeBase<T>> InAttribute, T InValue);

	template<typename T>
	static TWeakPtr<TAttributeBase<T>> GetAttributeFromCard(UCard* InCard, FName InName);
};

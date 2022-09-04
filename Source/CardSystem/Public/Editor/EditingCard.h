#pragma once

#include "CoreMinimal.h"

class FEditingCardFunction;

class FEditingCard
{
public:
	FEditingCard();

	void SetCardName(FString InName);

	FString GetCardName() const
	{
		return CardName;
	}

	void CreateNewFunction(const FString& InName);

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FEditingCardNameChangedSignature, FString)
	FEditingCardNameChangedSignature OnEditingCardNameChanged;

	DECLARE_MULTICAST_DELEGATE_OneParam(FEditingCardAddedFunctionSignature, TWeakPtr<FEditingCardFunction>)
	FEditingCardAddedFunctionSignature OnEditingCardFunctionAdded;

private:
	FString CardName;

	TMap<int32, TSharedPtr<FEditingCardFunction>> FunctionMap;
};

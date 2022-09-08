#pragma once

#include "CoreMinimal.h"

class FEditingCardCommand;

class FEditingCardFunction
{
public:
	FEditingCardFunction();

	void SetName(FString InName);

	void SetID(int32 InId);

	int32 GetID() const
	{
		return ID;
	}

	FString GetName() const
	{
		return FunctionName;
	}

	TWeakPtr<FEditingCardCommand> AddCommand();

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FEditingFunctionNameChangedSignature, FString)
	FEditingFunctionNameChangedSignature OnEditingFunctionNameChanged;

private:
	int32 ID;

	FString FunctionName;

	TArray<TSharedPtr<FEditingCardCommand>> CommandList;
};

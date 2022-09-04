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

private:
	int32 ID;

	FString FunctionName;

	TArray<TSharedPtr<FEditingCardCommand>> CommandList;
};

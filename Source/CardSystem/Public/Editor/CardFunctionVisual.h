#pragma once

#include "CoreMinimal.h"
#include "CardFunctionVisual.generated.h"

class FEditingCardFunction;

UCLASS(MinimalAPI)
class UCardFunctionVisual : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(int32 InId, FString InName, TWeakPtr<FEditingCardFunction> InFunction)
	{
		ID = InId;
		FunctionName = InName;
		EditingFunction = InFunction;
	}

	int32 GetID() const
	{
		return ID;
	}

	FString GetFunctionName() const
	{
		return FunctionName;
	}

	TWeakPtr<FEditingCardFunction> GetFunction()
	{
		return EditingFunction;
	}

protected:
	int32 ID;

	FString FunctionName;
	TWeakPtr<FEditingCardFunction> EditingFunction;
};

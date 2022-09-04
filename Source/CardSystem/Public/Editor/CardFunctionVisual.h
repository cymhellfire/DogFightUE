#pragma once

#include "CoreMinimal.h"
#include "CardFunctionVisual.generated.h"

UCLASS(MinimalAPI)
class UCardFunctionVisual : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(int32 InId, FString InName)
	{
		ID = InId;
		FunctionName = InName;
	}

	int32 GetID() const
	{
		return ID;
	}

	FString GetFunctionName() const
	{
		return FunctionName;
	}

protected:
	int32 ID;

	FString FunctionName;
};

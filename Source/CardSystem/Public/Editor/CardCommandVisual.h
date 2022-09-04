#pragma once

#include "CoreMinimal.h"
#include "Common/CardSystemType.h"
#include "CardCommandVisual.generated.h"

UCLASS(MinimalAPI)
class UCardCommandVisual : public UObject
{
	GENERATED_BODY()

public:

	void Initialize(const int32 InId, const FText& InName, const FText& InDesc, const ECardCommandType Type);

	int32 GetCommandId() const
	{
		return ID;
	}

	const FText& GetCommandName() const
	{
		return CommandName;
	}

	const FText& GetCommandDesc() const
	{
		return CommandDesc;
	}

	int32 GetCommandType() const
	{
		return CommandType;
	}

protected:
	int32 ID;

	FText CommandName;

	FText CommandDesc;

	ECardCommandType CommandType;
};

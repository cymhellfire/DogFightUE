#pragma once

#include "CoreMinimal.h"

class FEditingCardCommand
{
public:
	FEditingCardCommand();

	int32 GetCommandID() const
	{
		return CommandID;
	}

private:
	int32 CommandID;
};

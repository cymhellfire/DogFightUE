#pragma once

#include "CoreMinimal.h"

class FEditingCardCommand
{
public:
	FEditingCardCommand();

	void SetCommandID(int32 InId);

	void SetSelectIndex(int32 InIndex)
	{
		SelectIndex = InIndex;
	}

	int32 GetCommandID() const
	{
		return CommandID;
	}

	int32 GetSelectIndex() const
	{
		return SelectIndex;
	}

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FEditingCommandIdChangedSignature, int32);
	FEditingCommandIdChangedSignature OnCommandIdChanged;

private:
	int32 CommandID;

	int32 SelectIndex;
};

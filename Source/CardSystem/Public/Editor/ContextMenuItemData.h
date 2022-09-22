#pragma once

#include "CoreMinimal.h"
#include "ContextMenuItemData.generated.h"

UCLASS(MinimalAPI)
class UContextMenuItemData : public UObject
{
	GENERATED_BODY()
public:
	FText ItemName;

	TFunction<void()> ItemCallback;
};

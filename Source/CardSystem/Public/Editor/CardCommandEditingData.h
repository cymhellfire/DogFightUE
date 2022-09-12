#pragma once

#include "CoreMinimal.h"
#include "CardCommandEditingData.generated.h"

class FEditingCardCommand;

UCLASS(MinimalAPI)
class UCardCommandEditingData : public UObject
{
	GENERATED_BODY()
public:

	TWeakPtr<FEditingCardCommand> EditingCommand;
};

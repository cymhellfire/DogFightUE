#pragma once

#include "Engine/DataTable.h"
#include "CardCommandTableRow.generated.h"

class UCardCommand;

USTRUCT(BlueprintType)
struct FCardCommandTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CardCommand")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CardCommand")
	TSubclassOf<UCardCommand> CommandClass;
};

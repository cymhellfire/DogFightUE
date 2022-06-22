#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Test/PrintTargetNameCardCommand.h"
#include "CardCommandLibrary.generated.h"

class UCard;

UCLASS()
class CARDSYSTEM_API UCardCommandLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	static void PrintTargetName(UCard* Card, const FPrintTargetNameCardCommandParam& Param);

protected:
	template<typename T>
	static void CreateCommand(UCard* Owner, void* CreateParams);
};

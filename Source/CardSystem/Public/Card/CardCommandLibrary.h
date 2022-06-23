#pragma once

#include "Card.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Test/PrintTargetNameCardCommand.h"
#include "Test/WaitAndRandomCallbackCardCommand.h"
#include "Test/WaitForTimeCardCommand.h"
#include "CardCommandLibrary.generated.h"

#define IMPLEMENT_NEW_COMMAND(CommandName)												\
	{																					\
		if (!IsValid(Card))																\
			return;																		\
		CreateCommand<U##CommandName##CardCommand>(Card, (void*)(&Param));				\
	}

UCLASS()
class CARDSYSTEM_API UCardCommandLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	static void PrintTargetName(UCard* Card, const FPrintTargetNameCardCommandParam& Param)
	IMPLEMENT_NEW_COMMAND(PrintTargetName)

	UFUNCTION(BlueprintCallable)
	static void WaitForTime(UCard* Card, const FWaitForTimeCardCommandParam& Param)
	IMPLEMENT_NEW_COMMAND(WaitForTime)

	UFUNCTION(BlueprintCallable)
	static void WaitAndRandomCallback(UCard* Card, const FWaitAndRandomCallbackCardCommandParam& Param)
	IMPLEMENT_NEW_COMMAND(WaitAndRandomCallback)

protected:
	template<typename T>
	static void CreateCommand(UCard* Owner, void* CreateParams);
};

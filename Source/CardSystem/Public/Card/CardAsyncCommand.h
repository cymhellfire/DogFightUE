#pragma once

#include "CardCommand.h"
#include "CardAsyncCommand.generated.h"

UCLASS(Abstract)
class UCardAsyncCommand : public UCardCommand
{
	GENERATED_BODY()
public:
	UCardAsyncCommand()
	{
		ExecuteType = ECardCommandExecuteType::CCET_Asynchronous;
	}

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCardAsyncCommandFinishedSignature, UCardAsyncCommand*, Command, bool, bSuccess);
	FCardAsyncCommandFinishedSignature OnCommandFinished;
};

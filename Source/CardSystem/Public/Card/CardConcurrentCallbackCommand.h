#pragma once

#include "CardCommand.h"
#include "CardConcurrentCallbackCommand.generated.h"

UCLASS()
class UCardConcurrentCallbackCommand : public UCardCommand
{
	GENERATED_BODY()
public:
	UCardConcurrentCallbackCommand()
	{
		ExecuteType = ECardCommandExecuteType::CCET_ConcurrentCallback;
	}

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCardConcurrentCallbackCommandFinishSignature, UCardConcurrentCallbackCommand*, Command, int32, Result);
	FCardConcurrentCallbackCommandFinishSignature OnCommandFinished;
};

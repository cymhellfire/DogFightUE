#pragma once

#include "CoreMinimal.h"
#include "CardCommand.generated.h"

UENUM()
enum class ECardCommandExecuteType : uint8
{
	CCET_Synchronous,				// This command will be executed immediately.
	CCET_Asynchronous,				// This command will block all commands behind until it finished.
	CCET_ConcurrentCallback,		// This command will not block others behind it, and triggers a callback when it finished.
};

class UCard;

#define CAST_INIT_PARAM(Param, OutParam, NewType)								\
	NewType* OutParam = reinterpret_cast<NewType*>(Param);			\
	if (OutParam == nullptr)										\
		return false;

UCLASS(Abstract)
class CARDSYSTEM_API UCardCommand : public UObject
{
	GENERATED_BODY()
public:
	UCardCommand()
	{
		ExecuteType = ECardCommandExecuteType::CCET_Synchronous;
	}

	virtual void Run() PURE_VIRTUAL(UCardCommand::Run,);

	virtual bool Initialize(void* Param);

	ECardCommandExecuteType GetExecuteType() const
	{
		return ExecuteType;
	}

protected:
	ECardCommandExecuteType ExecuteType;

	TWeakObjectPtr<UCard> OwnerCard;
};

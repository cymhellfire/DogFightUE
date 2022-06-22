#pragma once

#include "CoreMinimal.h"
#include "CardCommand.generated.h"

UENUM()
enum class ECardCommandExecuteType : uint8
{
	CCET_Synchronous,
	CCET_Asynchronous,
};

class UCard;

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

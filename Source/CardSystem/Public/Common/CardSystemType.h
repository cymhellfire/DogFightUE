#pragma once

#include "CoreMinimal.h"
#include "CardSystemType.generated.h"

UENUM(BlueprintType)
enum ECardCommandType
{
	CCT_Default,
};

UENUM(BlueprintType)
namespace ECardTargetAcquireType
{
	enum Type
	{
		Success,
		Failed,
		Cancel,
	};
}

UENUM(BlueprintType)
namespace ECardLogicFinishType
{
	enum Type
	{
		Success,
		Failed,
		Cancel,
	};
}

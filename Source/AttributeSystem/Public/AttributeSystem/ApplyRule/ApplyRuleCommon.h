#pragma once

#include "CoreMinimal.h"
#include "ApplyRuleCommon.generated.h"

UENUM()
enum EApplyRuleGroupType
{
	ARGT_PassAll,
	ARGT_PassOne,
};

UENUM()
enum ETagMatchMethod
{
	TMM_ContainAll,
	TMM_ContainOne,
};

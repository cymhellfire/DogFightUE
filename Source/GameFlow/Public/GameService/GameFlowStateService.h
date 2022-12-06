#pragma once

#include "GameService/LuaGameService.h"
#include "GameFlowStateService.generated.h"

class UGameFlowStateBase;

UCLASS()
class UGameFlowStateService : public ULuaGameService
{
	GENERATED_BODY()
public:

	UGameFlowStateBase* CreateGameFlowState(FName InName);
};

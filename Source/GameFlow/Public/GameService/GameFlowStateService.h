#pragma once

#include "GameService/LuaGameService.h"
#include "GameFlowStateService.generated.h"

class UGameFlowStateBase;
class UGameFlowStateMachine;
class UGameFlowStateCreateArgument;

UCLASS()
class GAMEFLOW_API UGameFlowStateService : public ULuaGameService
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category="GameFlowStateService")
	UGameFlowStateMachine* CreateStateMachine(const FString& InPath, UObject* Instigator);

	UFUNCTION(BlueprintImplementableEvent, Category="GameFlowStateService")
	UGameFlowStateBase* CreateGameFlowState(UGameFlowStateCreateArgument* InArgument);

	UFUNCTION(BlueprintImplementableEvent, Category="GameFlowStateService")
	UGameFlowStateCreateArgument* GetGameFlowStateCreateArgument(UObject* Instigator);

	virtual FString GetModuleName_Implementation() const override
	{
		return TEXT("GameFlow.Services.GameFlowStateService");
	}
};

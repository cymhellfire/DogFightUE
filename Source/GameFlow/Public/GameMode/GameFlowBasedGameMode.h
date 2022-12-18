#pragma once

#include "GameMode/DogFightGameModeBase.h"
#include "GameFlowBasedGameMode.generated.h"

class UGameFlowStateMachine;

UCLASS(Abstract)
class GAMEFLOW_API AGameFlowBasedGameMode : public ADogFightGameModeBase
{
	GENERATED_BODY()
public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameFlowBasedGameMode")
	FString StateMachineName;

protected:
	UPROPERTY(Transient)
	UGameFlowStateMachine* GameFlowStateMachine;
};

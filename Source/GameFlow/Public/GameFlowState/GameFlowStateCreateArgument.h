#pragma once

#include "CoreMinimal.h"
#include "GameFlowStateCreateArgument.generated.h"

UCLASS()
class GAMEFLOW_API UGameFlowStateCreateArgument : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="GameFlowStateCreateArgument")
	FName StateName;

	UPROPERTY(BlueprintReadWrite, Category="GameFlowStateCreateArgumnet")
	UObject* Instigator;
};

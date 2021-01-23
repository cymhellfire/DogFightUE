#pragma once

#include "CoreMinimal.h"
#include "BuffableActorInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UBuffableActorInterface : public UInterface
{
	GENERATED_BODY()
};

class IBuffableActorInterface
{
	GENERATED_BODY()

public:

	virtual class UBuffQueue* GetBuffQueue() { return nullptr; }
};

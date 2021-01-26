#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"

#include "GameplayTagsActorInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UGameplayTagsActorInterface : public UInterface
{
	GENERATED_BODY()
};

class IGameplayTagsActorInterface
{
	GENERATED_BODY()

public:
	/** Get the GameplayTags of this actor. */
	virtual void GetGameplayTags(FGameplayTagContainer& OutGameplayTags) = 0;
};

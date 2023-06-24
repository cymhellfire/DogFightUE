// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem/LuaGameInstanceSubsystem.h"
#include "GameplayDataSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UGameplayDataSubsystem : public ULuaGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Subsystem.GameplayDataSubsystem";
	}
};

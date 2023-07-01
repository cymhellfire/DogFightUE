// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem/LuaGameInstanceSubsystem.h"
#include "PlayerDataSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLocalPlayerInitFinishEvent);

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UPlayerDataSubsystem : public ULuaGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Initialize local player for play.
	 */
	UFUNCTION(BlueprintCallable, Category="PlayerDataSubsystem")
	void TryToInitializeLocalPlayer();

	virtual FString GetModuleName_Implementation() const override
	{
		return "";
	}

protected:

public:
	UPROPERTY(BlueprintAssignable, Category="PlayerDataSubsystem")
	FLocalPlayerInitFinishEvent OnLocalPlayerInit;

protected:
};

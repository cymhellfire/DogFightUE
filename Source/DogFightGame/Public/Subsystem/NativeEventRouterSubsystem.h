// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem/LuaGameInstanceSubsystem.h"
#include "NativeEventRouterSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UNativeEventRouterSubsystem : public ULuaGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

protected:
	void RegisterCallbacks();

	void UnRegisterCallbacks();

	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	void HandlePostLoadMapWithWorld(UWorld* World);
};

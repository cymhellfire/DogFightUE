// Dog Fight Game Code By CYM.


#include "Subsystem/NativeEventRouterSubsystem.h"

#include "Common/LuaEventDef.h"
#include "GameService/LuaEventService.h"

void UNativeEventRouterSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	RegisterCallbacks();
}

void UNativeEventRouterSubsystem::Deinitialize()
{
	Super::Deinitialize();

	UnRegisterCallbacks();
}


void UNativeEventRouterSubsystem::RegisterCallbacks()
{
	GEngine->OnNetworkFailure().AddUObject(this, &UNativeEventRouterSubsystem::HandleNetworkFailure);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UNativeEventRouterSubsystem::HandlePostLoadMapWithWorld);
}

void UNativeEventRouterSubsystem::UnRegisterCallbacks()
{
	GEngine->OnNetworkFailure().RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

void UNativeEventRouterSubsystem::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver,
	ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	SEND_LUA_EVENT(ELuaEvent::LuaEvent_NetworkFailure, FailureType)
}

void UNativeEventRouterSubsystem::HandlePostLoadMapWithWorld(UWorld* World)
{
	SEND_LUA_EVENT(ELuaEvent::LuaEvent_PostLoadMap, World)
}

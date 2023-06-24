// Dog Fight Game Code By CYM.


#include "Subsystem/LuaGameInstanceSubsystem.h"

#include "GameInstance/LuaGameInstance.h"

void ULuaGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Invoke script initialize
	ScriptInit();
}

void ULuaGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();

	// Invoke script deinitialize
	ScriptDeinit();
}

ULuaGameInstance* ULuaGameInstanceSubsystem::GetLuaGameInstance() const
{
	return Cast<ULuaGameInstance>(GetGameInstance());
}

#include "GameService/LuaGameService.h"

#include "Common/LuaIntegrationLog.h"

void ULuaGameService::Startup()
{
	Super::Startup();

	// Invoke lua side function
	StartupScript(GetRegisterName().ToString());
}

void ULuaGameService::Shutdown()
{
	Super::Shutdown();

	// Invoke lua side function
	ShutdownScript(GetRegisterName().ToString());
}

FName ULuaGameService::GetRegisterName() const
{
	UClass* MyClass = GetClass();
	FString RegisterName = MyClass->GetName();
	if (MyClass->HasAnyClassFlags(CLASS_CompiledFromBlueprint) && bRegisterNativeClassName)
	{
		while (IsValid(MyClass) && MyClass->HasAnyClassFlags(CLASS_CompiledFromBlueprint))
		{
			MyClass = MyClass->GetSuperClass();
		}
		if (IsValid(MyClass))
		{
			RegisterName = MyClass->GetName();
		}
		else
		{
			UE_LOG(LogLuaIntegration, Error, TEXT("[LuaGameService] Cannot find native super class for %s"), *RegisterName);
		}
	}

	return FName(RegisterName);
}

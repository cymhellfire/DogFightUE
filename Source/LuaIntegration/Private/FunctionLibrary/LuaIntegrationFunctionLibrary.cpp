#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"

UWorld* ULuaIntegrationFunctionLibrary::GetCurrentWorld()
{
	UWorld* CurWorld = nullptr;
	if (GWorld)
	{
		CurWorld = GWorld->GetWorld();
		UE_LOG(LogTemp, Log, TEXT("Debug-GWorld: %s %p"), *CurWorld->GetName(), CurWorld);
	}
	return CurWorld;
}

UGameInstance* ULuaIntegrationFunctionLibrary::GetGameInstance()
{
	if (auto CurrentWorld = GetCurrentWorld())
	{
		return CurrentWorld->GetGameInstance();
	}

	return nullptr;
}

UClass* ULuaIntegrationFunctionLibrary::LoadClassByPath(FString InPath)
{
	if (InPath.IsEmpty())
	{
		return nullptr;
	}

	UClass* LoadedClass = LoadClass<UObject>(nullptr, *InPath);

	return LoadedClass;
}

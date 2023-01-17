#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"

UWorld* ULuaIntegrationFunctionLibrary::GetCurrentWorld()
{
	if (auto WorldContext = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport))
	{
		return WorldContext->World();
	}

	return nullptr;
}

UGameInstance* ULuaIntegrationFunctionLibrary::GetGameInstance()
{
	if (auto CurrentWorld = GetCurrentWorld())
	{
		return CurrentWorld->GetGameInstance();
	}

	return nullptr;
}

UClass* ULuaIntegrationFunctionLibrary::LoadClassByPath(FString InPath, UObject* InOuter)
{
	if (InPath.IsEmpty())
	{
		return nullptr;
	}

	UClass* LoadedClass = LoadClass<UObject>(nullptr, *InPath);
	if (LoadedClass)
	{
		UE_LOG(LogTemp, Log, TEXT("Load finished."));
	}

	return LoadedClass;
}

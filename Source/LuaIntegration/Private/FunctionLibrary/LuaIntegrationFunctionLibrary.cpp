#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"

#include "Common/LuaIntegrationLog.h"
#include "GameInstance/DogFightGameInstance.h"
#include "GameService/LocalizationService.h"

UWorld* ULuaIntegrationFunctionLibrary::GetCurrentWorld(const UObject* WorldContextObject)
{
// #if WITH_EDITOR
// 	// Use GWorld here to ensure simulating multiplayer game locally in editor can work
// 	UWorld* CurWorld = nullptr;
//
// 	// Cache the PIE instance id once it's updated
// 	static int32 LastUpdatePIEInstance = -1;
// 	if (LastUpdatePIEInstance != GPlayInEditorID)
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("Update LastUpdatePIEInstance from %d to %d"), LastUpdatePIEInstance, GPlayInEditorID);
// 		LastUpdatePIEInstance = GPlayInEditorID;
// 	}
//
// 	if (LastUpdatePIEInstance != -1)
// 	{
// 		if (auto WorldContext = GEngine->GetWorldContextFromPIEInstance(LastUpdatePIEInstance))
// 		{
// 			return WorldContext->World();
// 		}
// 	}
// 	return GWorld;
// #else
// 	const FWorldContext* Context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
// 	if (Context)
// 	{
// 		return Context->World();
// 	}
//
// 	return nullptr;
// #endif
	return IsValid(WorldContextObject) ? WorldContextObject->GetWorld() : nullptr;
}

UGameInstance* ULuaIntegrationFunctionLibrary::GetGameInstance(const UObject* WorldContextObject)
{
	if (auto CurrentWorld = GetCurrentWorld(WorldContextObject))
	{
		return CurrentWorld->GetGameInstance();
	}

	return nullptr;
}

APlayerController* ULuaIntegrationFunctionLibrary::GetFirstLocalPlayerController(const UObject* WorldContextObject)
{
	if (auto GameInstance = GetGameInstance(WorldContextObject))
	{
		return GameInstance->GetFirstLocalPlayerController();
	}

	return nullptr;
}

UClass* ULuaIntegrationFunctionLibrary::LoadClassByPath(FString InPath)
{
	if (InPath.IsEmpty())
	{
		return nullptr;
	}

	// Ensure legal format
	int32 DotIndex = -1;
	InPath.FindLastChar('.', DotIndex);
	if (DotIndex == -1)
	{
		int32 SlashIndex = -1;
		InPath.FindLastChar('/', SlashIndex);
		if (SlashIndex == -1)
		{
			UE_LOG(LogLuaIntegration, Error, TEXT("[LuaIntegrationFunctionLibrary] Invalid class path to load: %s"), *InPath);
			return nullptr;
		}
		FString Suffix = InPath.Mid(SlashIndex + 1, InPath.Len() - SlashIndex - 1);
		InPath = InPath + FString::Printf(TEXT(".%s_C"), *Suffix);
	}

	UClass* LoadedClass = LoadClass<UObject>(nullptr, *InPath);

	return LoadedClass;
}

bool ULuaIntegrationFunctionLibrary::IsDerivedFrom(UObject* InObject, UClass* InClass)
{
	if (IsValid(InObject) && IsValid(InClass))
	{
		return InObject->GetClass()->IsChildOf(InClass);
	}
	return false;
}

FText ULuaIntegrationFunctionLibrary::GetLocalizedString(const UObject* WorldContextObject, const FString& InTable, const FString& InKey)
{
	if (auto GameInstance = Cast<UDogFightGameInstance>(GetGameInstance(WorldContextObject)))
	{
		if (auto LocalizationService = Cast<ULocalizationService>(GameInstance->GetGameServiceBySuperClass(ULocalizationService::StaticClass())))
		{
			auto Result = LocalizationService->GetLocalizeString(InTable, InKey);
			if (!Result.IsEmpty())
			{
				return Result;
			}
		}
	}

	return FText::FromString(FString::Printf(TEXT("Missing localization string: [%s - %s]"), *InTable, *InKey));
}

FText ULuaIntegrationFunctionLibrary::GetLocalizedStringWithParam(const UObject* WorldContextObject, const FString& InTable,
	const FString& InKey, const TArray<FString>& ParamList)
{
	if (auto GameInstance = Cast<UDogFightGameInstance>(GetGameInstance(WorldContextObject)))
	{
		if (auto LocalizationService = Cast<ULocalizationService>(GameInstance->GetGameServiceBySuperClass(ULocalizationService::StaticClass())))
		{
			auto Result = LocalizationService->GetLocalizeStringWithFormat(InTable, InKey, ParamList);
			if (!Result.IsEmpty())
			{
				return Result;
			}
		}
	}

	return FText::FromString(FString::Printf(TEXT("Missing localization string: [%s - %s]"), *InTable, *InKey));
}

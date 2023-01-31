#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"

#include "GameInstance/DogFightGameInstance.h"
#include "GameService/LocalizationService.h"

UWorld* ULuaIntegrationFunctionLibrary::GetCurrentWorld()
{
#if WITH_EDITOR
	// Use GWorld here to ensure simulating multiplayer game locally in editor can work
	UWorld* CurWorld = nullptr;
	if (GWorld)
	{
		CurWorld = GWorld->GetWorld();
	}
	return CurWorld;
#else
	const FWorldContext* Context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
	if (Context)
	{
		return Context->World();
	}

	return nullptr;
#endif
}

UGameInstance* ULuaIntegrationFunctionLibrary::GetGameInstance()
{
	if (auto CurrentWorld = GetCurrentWorld())
	{
		return CurrentWorld->GetGameInstance();
	}

	return nullptr;
}

APlayerController* ULuaIntegrationFunctionLibrary::GetFirstLocalPlayerController()
{
	if (auto GameInstance = GetGameInstance())
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

FText ULuaIntegrationFunctionLibrary::GetLocalizedString(const FString& InTable, const FString& InKey)
{
	if (auto GameInstance = Cast<UDogFightGameInstance>(GetGameInstance()))
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

FText ULuaIntegrationFunctionLibrary::GetLocalizedStringWithParam(const FString& InTable, const FString& InKey,
	const TArray<FString>& ParamList)
{
	if (auto GameInstance = Cast<UDogFightGameInstance>(GetGameInstance()))
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

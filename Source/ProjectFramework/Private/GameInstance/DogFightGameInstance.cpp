// Dog Fight Game Code By CYM.


#include "GameInstance/DogFightGameInstance.h"

#include "GameMode/DogFightGameModeBase.h"
#include "GameMode/DogFightGameStateBase.h"
#include "Common/FrameworkType.h"
#include "SaveGame/SaveGameManager.h"
#include "Blueprint/UserWidget.h"
#include "Common/Localization.h"
#include "GameService/GameService.h"
#include "Kismet/GameplayStatics.h"

UDogFightGameInstance::UDogFightGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set GamePlayerCount default value as 1
	GamePlayerCount = 1;
}

void UDogFightGameInstance::Shutdown()
{
	Super::Shutdown();

	// Remove the ticker delegate
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
}

UGameService* UDogFightGameInstance::GetGameService(FName ClassName)
{
	if (GameServiceMap.Contains(ClassName))
	{
		return GameServiceMap[ClassName];
	}

	UE_LOG(LogProjectFramework, Error, TEXT("[GameInstance] No game service with name: %s"), *ClassName.ToString());
	return nullptr;
}

UGameService* UDogFightGameInstance::GetGameServiceBySuperClass(UClass* SuperClass)
{
	TArray<UGameService*> AllServices;
	GameServiceMap.GenerateValueArray(AllServices);
	for (auto Service : AllServices)
	{
		if (Service->GetClass()->IsChildOf(SuperClass))
		{
			return Service;
		}
	}

	return nullptr;
}

void UDogFightGameInstance::Init()
{
	Super::Init();

	// Create Save Manager
	SaveGameManager = NewObject<USaveGameManager>(this, FName(TEXT("SaveGameManager")));

	// Register delegate for ticker callback
	// TickDelegate = FTickerDelegate::CreateUObject(this, &UDogFightGameInstance::Tick);
	// TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);

	// Startup game services
	StartupGameService();
}

bool UDogFightGameInstance::Tick(float DeltaSeconds)
{
	// Because this takes place outside the normal UWorld tick, we need to register what world we're ticking/modifying here to avoid issues in the editor
	FScopedConditionalWorldSwitcher WorldSwitcher(GetWorld());

	return true;
}

bool UDogFightGameInstance::LoadFrontEndMap(const FString& MapName)
{
	bool bSuccess = true;

	// Do nothing if the map is already loaded
	UWorld* const World = GetWorld();
	if (World)
	{
		FString const CurrentMapName = *World->PersistentLevel->GetOutermost()->GetName();
		if (CurrentMapName == MapName)
		{
			return bSuccess;
		}
	}

	FString Error;
	EBrowseReturnVal::Type BrowseRet = EBrowseReturnVal::Failure;
	const FURL URL(*FString::Printf(TEXT("%s"), *MapName));

	if (URL.Valid && !HasAnyFlags(RF_ClassDefaultObject))
	{
		BrowseRet = GetEngine()->Browse(*WorldContext, URL, Error);

		// Handle failure
		if (BrowseRet != EBrowseReturnVal::Success)
		{
			UE_LOG(LogLoad, Fatal, TEXT("%s"), *FString::Printf(TEXT("Failed to enter %s: %s. Please check the log for errors."), *MapName, *Error));
			bSuccess = false;
		}
	}

	return bSuccess;
}

void UDogFightGameInstance::StartupGameService()
{
	TArray<UClass*> InstantiatedClasses;
	// Instantiate all game service classes
	for (TObjectIterator<UClass> It; It; ++It)
	{
		if (It->IsChildOf(UGameService::StaticClass()) && !It->HasAnyClassFlags(CLASS_Abstract))
		{
			UClass* NewServiceClass = *It;
			if (NewServiceClass->HasAnyClassFlags(CLASS_CompiledFromBlueprint))
			{
				continue;
			}
			
			UGameService* NewGameService = NewObject<UGameService>(this, NewServiceClass, It->GetFName());
			GameServiceMap.Add(NewGameService->GetRegisterName(), NewGameService);

			// Invoke Startup function
			NewGameService->Startup();

			UE_LOG(LogProjectFramework, Log, TEXT("[GameService] %s startup."), *It->GetName());
			InstantiatedClasses.Add(*It);
		}
	}

	// Instantiate blueprinted services
	for (auto Blueprint : ServiceBlueprints)
	{
		if (InstantiatedClasses.Contains(Blueprint))
			continue;

		UGameService* NewGameService = NewObject<UGameService>(this, Blueprint, Blueprint->GetFName());
		GameServiceMap.Add(NewGameService->GetRegisterName(), NewGameService);

		// Startup
		NewGameService->Startup();

		UE_LOG(LogProjectFramework, Log, TEXT("[GameService] %s startup."), *Blueprint->GetName());
		InstantiatedClasses.Add(Blueprint);
	}
}

void UDogFightGameInstance::ShowLoadingScreen()
{
	// Create Loading screen widget
	const FSoftClassPath WidgetClassRef(TEXT("/Game/DogFight/UI/TransitionMap/BP_TransitionMapWidget.BP_TransitionMapWidget_C"));
	if (UClass* WidgetClass = WidgetClassRef.TryLoadClass<UUserWidget>())
	{
		UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetFirstLocalPlayerController(), WidgetClass);
		NewWidget->AddToViewport();
	}
}

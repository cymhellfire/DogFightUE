// Dog Fight Game Code By CYM.


#include "SaveGameManager.h"


#include "DogFight.h"
#include "DogFightSaveGame.h"
#include "Kismet/GameplayStatics.h"

TArray<FString> USaveGameManager::GetAllSaveGame()
{
	class FFindSaveGameVisitor : public IPlatformFile::FDirectoryVisitor
	{
	public:
		FFindSaveGameVisitor() {}

		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
		{
			if (!bIsDirectory)
			{
				const FString FullFilePath(FilenameOrDirectory);
				if (FPaths::GetExtension(FullFilePath) == TEXT("sav"))
				{
					FString CleanFilename = FPaths::GetBaseFilename(FullFilePath);
					CleanFilename = CleanFilename.Replace(TEXT(".sav"), TEXT(""));
					FoundSaves.Add(CleanFilename);
				}
			}
			return true;
		}

		TArray<FString> FoundSaves;
	};

	TArray<FString> SaveSlotNameList; 
	const FString SavesFolder = FPaths::ProjectSavedDir() + TEXT("SaveGames");

	if (!SavesFolder.IsEmpty())
	{
		FFindSaveGameVisitor Visitor;
		FPlatformFileManager::Get().GetPlatformFile().IterateDirectory(*SavesFolder, Visitor);
		SaveSlotNameList = Visitor.FoundSaves;

		// Cache result
		CacheSlotList = Visitor.FoundSaves;
	}

	return SaveSlotNameList;
}

bool USaveGameManager::SaveAtSlot(FString PlayerName, FString OldSlotName)
{
	// Check whether override current slot or create new slot
	if (CurrentSaveGameInstance != nullptr)
	{
		CurrentSaveGameInstance->PlayerName = PlayerName;

		// Check old slot exists
		if (!OldSlotName.IsEmpty() && UGameplayStatics::DoesSaveGameExist(OldSlotName, 0))
		{
			UGameplayStatics::DeleteGameInSlot(OldSlotName, 0);
		}

		// Save data synchronous
		if (UGameplayStatics::SaveGameToSlot(CurrentSaveGameInstance, PlayerName, CurrentSaveGameInstance->UserIndex))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Profile %s saved successful."), *PlayerName));
		}
	}
	else
	{
		if (UDogFightSaveGame* SaveGameInstance = Cast<UDogFightSaveGame>(UGameplayStatics::CreateSaveGameObject(UDogFightSaveGame::StaticClass())))
		{
			SaveGameInstance->PlayerName = PlayerName;

			// Save data synchronous
			if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, PlayerName, 0))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Profile %s saved successful."), *PlayerName));
			}
		}
		else
		{
			UE_LOG(LogDogFight, Error, TEXT("Failed to create new SaveGame at slot: %s"), *PlayerName);
		}
	}
	
	return true;
}

void USaveGameManager::DeleteAtSlot(FString SlotName)
{
	if (!CacheSlotList.Contains(SlotName))
	{
		UE_LOG(LogDogFight, Error, TEXT("No save game in slot: %s"), *SlotName);
		return;
	}

	if (!UGameplayStatics::DeleteGameInSlot(SlotName, 0))
	{
		UE_LOG(LogDogFight, Error, TEXT("Failed to delete game in slot: %s"), *SlotName);
	}
}

void USaveGameManager::SelectSaveGame(FString SlotName)
{
	if (!CacheSlotList.Contains(SlotName))
	{
		UE_LOG(LogDogFight, Error, TEXT("No such save game %s."), *SlotName);
		return;
	}

	// Record select slot name
	SelectedSlotName = SlotName;

	// Load specified save game
	LoadSaveGameAtSlot(SelectedSlotName);

	SaveOptionFile();
}

USaveGameManager::USaveGameManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LoadOptionFile();
}

void USaveGameManager::LoadOptionFile()
{
	static FString FileName = FString("SaveGameOption.ini");
	static FString SaveDirectory = FPaths::ProjectSavedDir();
	
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.DirectoryExists(*SaveDirectory))
	{
		static FString AbsolutePath = SaveDirectory + "/" + FileName;

		// Load file content
		if (PlatformFile.FileExists(*AbsolutePath))
		{
			TArray<FString> FileContent;
			FFileHelper::LoadFileToStringArray(FileContent, *AbsolutePath);

			for (FString Line : FileContent)
			{
				TArray<FString> Parts;
				Line.ParseIntoArray(Parts, TEXT("="));
				if (Parts.Num() == 2)
				{
					SetProperty(Parts[0], Parts[1]);
				}
			}
		}
	}
}

void USaveGameManager::SetProperty(FString Key, FString Value)
{
	if (Key == TEXT("SelectedSlot"))
	{
		SelectedSlotName = Value;

		// Load the selected slot
		LoadSaveGameAtSlot(SelectedSlotName);
	}
	else
	{
		UE_LOG(LogDogFight, Error, TEXT("Invalid SaveGameOption property: %s"), *Key);
	}
}

void USaveGameManager::SaveOptionFile()
{
	static FString FileName = FString("SaveGameOption.ini");
	static FString SaveDirectory = FPaths::ProjectSavedDir();

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.CreateDirectoryTree(*SaveDirectory))
	{
		static FString AbsolutePath = SaveDirectory + "/" + FileName;

		// Generate file content
		TArray<FString> Content;
		Content.Add(FString::Printf(TEXT("SelectedSlot=%s"), *SelectedSlotName));
		
		FFileHelper::SaveStringArrayToFile(Content, *AbsolutePath);
	}
}

void USaveGameManager::LoadSaveGameAtSlot(FString SlotName)
{
	if (UDogFightSaveGame* LoadedGame = Cast<UDogFightSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0)))
	{
		CurrentSaveGameInstance = LoadedGame;

		UE_LOG(LogDogFight, Log, TEXT("Load save game %s successfully."), *SlotName);
	}
	else
	{
		UE_LOG(LogDogFight, Error, TEXT("Failed load save game at slot: %s"), *SlotName);
	}
}


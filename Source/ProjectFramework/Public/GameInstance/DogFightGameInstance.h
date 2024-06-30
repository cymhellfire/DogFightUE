// Dog Fight Game Code By CYM.

#pragma once

#include "ProjectFramework.h"
#include "Common/MainMenuMessage.h"
#include "Engine/GameInstance.h"
#include "Online.h"
#include "DogFightGameInstance.generated.h"

class UGameService;

/**
 * 
 */
UCLASS()
class PROJECTFRAMEWORK_API UDogFightGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UDogFightGameInstance(const FObjectInitializer& ObjectInitializer);

	/* Name of menu map. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=GameInstance)
	FString MenuMapName;

	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable, Category="GameInstance")
	UGameService* GetGameService(FName ClassName);

	UGameService* GetGameServiceBySuperClass(UClass* SuperClass);

	bool LoadFrontEndMap(const FString& MapName);

private:

	UPROPERTY(Transient)
	TMap<FName, UGameService*> GameServiceMap; 

public:
	/** Initialize Game Instance. */
	virtual void Init() override;

	bool Tick(float DeltaSeconds);

	void ShowLoadingScreen();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="DogFight|Game")
	int32 GamePlayerCount;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="DogFight|Game")
	int32 GameAICount;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="DogFight|Service")
	TArray<TSubclassOf<UGameService>> ServiceBlueprints;

private:
	void StartupGameService();

	FTickerDelegate TickDelegate;
	FTSTicker::FDelegateHandle TickDelegateHandle;
};

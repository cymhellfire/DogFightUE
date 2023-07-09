// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/GameplayExperience.h"
#include "Subsystem/LuaGameInstanceSubsystem.h"
#include "GameplayDataSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UGameplayDataSubsystem : public ULuaGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGameplayDataSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Subsystem.GameplayDataSubsystem";
	}

	UFUNCTION(BlueprintCallable, Category="GameplayDataSubsystem")
	bool IsGameExperienceLoaded() const
	{
		return bGameExperienceLoaded;
	}

	UFUNCTION(BlueprintCallable, Category="GameplayDataSubsystem")
	TArray<UGameplayExperience*> GetGameplayExperiencesByType(EGameplayExperienceType::Type InType);

	UFUNCTION(BlueprintCallable, Category="GameplayDataSubsystem")
	void LoadGameplayExperience(UGameplayExperience* GameplayExperience);

protected:

	void LoadAllGameExperiences();

protected:

	uint8 bGameExperienceLoaded : 1;
};

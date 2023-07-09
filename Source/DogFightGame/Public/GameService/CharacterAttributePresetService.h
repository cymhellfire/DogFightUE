// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameService/LuaGameService.h"
#include "CharacterAttributePresetService.generated.h"

class UGameplayAttributesComponent;

/**
 * Service that hold all character attribute configurations.
 */
UCLASS()
class DOGFIGHTGAME_API UCharacterAttributePresetService : public ULuaGameService
{
	GENERATED_BODY()

public:
	/**
	 * Load and initialize attribute container component.
	 * @param InComponent Attribute holder component to initialize.
	 * @param ConfigId Id of config to initialize with.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="CharacterAttributePresetService")
	void InitAttributePreset(UGameplayAttributesComponent* InComponent, int32 ConfigId);

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.CharacterAttributePresetService.CharacterAttributePresetService";
	}
};

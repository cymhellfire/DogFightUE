// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameService/LuaGameService.h"
#include "CharacterModifierService.generated.h"

class UCharacterStatusModifier;

/**
 * Service provide character modifier spawning and management.
 */
UCLASS()
class DOGFIGHTGAME_API UCharacterModifierService : public ULuaGameService
{
	GENERATED_BODY()

public:
	/**
	 * Create a new character status modifier with given id.
	 * @param InId			Id of character modifier to spawn.
	 * @return New spawned character modifier.
	 */
	UFUNCTION(BlueprintCallable, Category="CharacterModifierService")
	UCharacterStatusModifier* CreateCharacterStatusModifier(int32 InId);

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.CharacterModifierService.CharacterModifierService";
	}

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="CharacterModifierService")
	UCharacterStatusModifier* SpawnCharacterModifierInternal(int32 InId);
};

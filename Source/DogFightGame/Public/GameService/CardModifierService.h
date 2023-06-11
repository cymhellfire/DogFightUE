#pragma once

#include "CoreMinimal.h"
#include "GameService/LuaGameService.h"
#include "CardModifierService.generated.h"

class UCardAttributeModifier;

UCLASS()
class DOGFIGHTGAME_API UCardModifierService : public ULuaGameService
{
	GENERATED_BODY()
public:
	/**
	 * Create a new card modifier with given id.
	 * @param InId		Id of card modifier to spawn.
	 * @return New spawned card modifier.
	 */
	UFUNCTION(BlueprintCallable, Category="CardModifierService")
	UCardAttributeModifier* CreateCardModifier(int32 InId);

	virtual FString GetModuleName_Implementation() const override
	{
		return TEXT("DogFight.Services.CardModifierService.CardModifierService");
	}

protected:
	UFUNCTION(BlueprintImplementableEvent)
	UCardAttributeModifier* CreateCardModifierInternal(int32 InId);
};

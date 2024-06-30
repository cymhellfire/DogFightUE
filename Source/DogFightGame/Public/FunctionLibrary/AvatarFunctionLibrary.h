// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Pawn/ActionGameCharacter.h"
#include "AvatarFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UAvatarFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Setup avatar appearance with given config id.
	 * @param InAvatar Avatar instance to setup appearance.
	 * @param ConfigId Id of config to apply.
	 */
	UFUNCTION(BlueprintCallable, Category="AvatarFunctionLibrary", meta=(WorldContext="WorldContext"))
	static void InitAvatarAppearanceWithConfigId(UObject* WorldContext, AActionGameCharacter* InAvatar, int32 ConfigId);
};

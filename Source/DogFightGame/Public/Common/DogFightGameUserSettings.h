// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Subsystem/InGameStatistics/GameLobbyPlayerAvatarId.h"
#include "DogFightGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHTGAME_API UDogFightGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config)
	FString PlayerName;

	UPROPERTY(Config)
	FGameLobbyPlayerAvatarId LastAvatarId;
};

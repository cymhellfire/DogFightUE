#pragma once

#include "CoreMinimal.h"
#include "GameLobbyPlayerAvatarId.h"
#include "GameLobbyPlayerInfo.generated.h"

USTRUCT(BlueprintType)
struct FGameLobbyPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	bool bHost;

	UPROPERTY()
	FGameLobbyPlayerAvatarId AvatarId;

	friend bool operator==(const FGameLobbyPlayerInfo& Lhs, const FGameLobbyPlayerInfo& RHS)
	{
		return Lhs.PlayerName == RHS.PlayerName
			&& Lhs.bHost == RHS.bHost
			&& Lhs.AvatarId == RHS.AvatarId;
	}

	friend bool operator!=(const FGameLobbyPlayerInfo& Lhs, const FGameLobbyPlayerInfo& RHS)
	{
		return !(Lhs == RHS);
	}
};

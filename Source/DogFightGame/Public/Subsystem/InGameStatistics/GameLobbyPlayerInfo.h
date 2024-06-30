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

	FGameLobbyPlayerInfo()
		: bHost(false)
		, AvatarId(FGameLobbyPlayerAvatarId())
	{}

	FGameLobbyPlayerInfo(const FGameLobbyPlayerInfo& Other)
		: PlayerName(Other.PlayerName),
		  bHost(Other.bHost),
		  AvatarId(Other.AvatarId)
	{
	}

	FGameLobbyPlayerInfo(FGameLobbyPlayerInfo&& Other) noexcept
		: PlayerName(std::move(Other.PlayerName)),
		  bHost(Other.bHost),
		  AvatarId(std::move(Other.AvatarId))
	{
	}

	FGameLobbyPlayerInfo& operator=(const FGameLobbyPlayerInfo& Other)
	{
		if (this == &Other)
			return *this;
		PlayerName = Other.PlayerName;
		bHost = Other.bHost;
		AvatarId = Other.AvatarId;
		return *this;
	}

	FGameLobbyPlayerInfo& operator=(FGameLobbyPlayerInfo&& Other) noexcept
	{
		if (this == &Other)
			return *this;
		PlayerName = std::move(Other.PlayerName);
		bHost = Other.bHost;
		AvatarId = std::move(Other.AvatarId);
		return *this;
	}

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

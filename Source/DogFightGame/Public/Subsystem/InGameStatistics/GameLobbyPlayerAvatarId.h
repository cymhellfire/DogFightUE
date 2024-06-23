#pragma once

#include "CoreMinimal.h"
#include "GameLobbyPlayerAvatarId.generated.h"

UENUM(BlueprintType)
enum class EGameLobbyPlayerAvatarIdType : uint8
{
	EPAIT_None,
	EPAIT_AvatarId,
	EPAIT_GroupId,
};

USTRUCT(BlueprintType)
struct FGameLobbyPlayerAvatarId
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EGameLobbyPlayerAvatarIdType Type;

	UPROPERTY(EditAnywhere)
	int32 Id;

	friend bool operator==(const FGameLobbyPlayerAvatarId& Lhs, const FGameLobbyPlayerAvatarId& RHS)
	{
		return Lhs.Type == RHS.Type
			&& Lhs.Id == RHS.Id;
	}

	friend bool operator!=(const FGameLobbyPlayerAvatarId& Lhs, const FGameLobbyPlayerAvatarId& RHS)
	{
		return !(Lhs == RHS);
	}
};
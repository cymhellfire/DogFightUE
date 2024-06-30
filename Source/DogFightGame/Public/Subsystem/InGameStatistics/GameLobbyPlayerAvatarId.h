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

	FGameLobbyPlayerAvatarId()
		: Type(EGameLobbyPlayerAvatarIdType::EPAIT_None)
		, Id(0)
	{}

	FGameLobbyPlayerAvatarId(const FGameLobbyPlayerAvatarId& Other)
		: Type(Other.Type),
		  Id(Other.Id)
	{
	}

	FGameLobbyPlayerAvatarId(FGameLobbyPlayerAvatarId&& Other) noexcept
		: Type(Other.Type),
		  Id(Other.Id)
	{
	}

	FGameLobbyPlayerAvatarId& operator=(const FGameLobbyPlayerAvatarId& Other)
	{
		if (this == &Other)
			return *this;
		Type = Other.Type;
		Id = Other.Id;
		return *this;
	}

	FGameLobbyPlayerAvatarId& operator=(FGameLobbyPlayerAvatarId&& Other) noexcept
	{
		if (this == &Other)
			return *this;
		Type = Other.Type;
		Id = Other.Id;
		return *this;
	}

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
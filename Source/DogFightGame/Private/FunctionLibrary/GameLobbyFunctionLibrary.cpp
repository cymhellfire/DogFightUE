// Dog Fight Game Code By CYM.


#include "FunctionLibrary/GameLobbyFunctionLibrary.h"

#include "CommonSessionSubsystem.h"
#include "GameMode/DogFightGameModeBase.h"
#include "GameMode/GameLobbyGameState.h"
#include "Player/GameLobbyPlayerState.h"
#include "PlayerController/GameLobbyPlayerController.h"

TArray<AGameLobbyPlayerState*> UGameLobbyFunctionLibrary::GetAllGameLobbyPlayerState(const UObject* WorldContext)
{
	struct FSortPlayerStateById
	{
		bool operator()(const AGameLobbyPlayerState& A, const AGameLobbyPlayerState& B) const
		{
			const auto PlayerIdA = A.GetPlayerId();
			const auto PlayerIdB = B.GetPlayerId();
			return PlayerIdA > PlayerIdB;
		}
	};

	if (auto GS = GetCurrentLobbyGameState(WorldContext))
	{
		auto PlayerStateList = GS->GetAllPlayerState();
		PlayerStateList.Sort(FSortPlayerStateById());
		return PlayerStateList;
	}

	return TArray<AGameLobbyPlayerState*>();
}

AGameLobbyPlayerController* UGameLobbyFunctionLibrary::GetLocalGameLobbyPlayerController(const UObject* WorldContext)
{
	if (auto World = WorldContext->GetWorld())
	{
		return Cast<AGameLobbyPlayerController>(GEngine->GetFirstLocalPlayerController(World));
	}

	return nullptr;
}

AGameLobbyPlayerState* UGameLobbyFunctionLibrary::GetLocalGameLobbyPlayerState(const UObject* WorldContext)
{
	if (auto PC = GetLocalGameLobbyPlayerController(WorldContext))
	{
		return PC->GetPlayerState<AGameLobbyPlayerState>();
	}

	return nullptr;
}

AGameLobbyGameState* UGameLobbyFunctionLibrary::GetCurrentLobbyGameState(const UObject* WorldContext)
{
	if (!IsValid(WorldContext))
		return nullptr;

	if (auto CurWorld = WorldContext->GetWorld())
	{
		return Cast<AGameLobbyGameState>(CurWorld->GetGameState());
	}

	return nullptr;
}

UCommonSessionSubsystem* UGameLobbyFunctionLibrary::GetCommonSessionSubSystem(const UObject* WorldContext)
{
	if (IsValid(WorldContext))
	{
		if (auto CurWorld = WorldContext->GetWorld())
		{
			if (auto GameInstance = CurWorld->GetGameInstance())
			{
				return GameInstance->GetSubsystem<UCommonSessionSubsystem>();
			}
		}
	}

	return nullptr;
}

void UGameLobbyFunctionLibrary::DismissGameLobby(const UObject* WorldContext)
{
	if (!IsValid(WorldContext))
	{
		return;
	}

	if (auto CurWorld = WorldContext->GetWorld())
	{
		if (auto GM = Cast<ADogFightGameModeBase>(CurWorld->GetAuthGameMode()))
		{
			GM->DismissCurrentGameSession();
		}
		else if (auto PC = Cast<ADogFightPlayerController>(GEngine->GetFirstLocalPlayerController(CurWorld)))
		{
			PC->HandleReturnToMainMenu2();
		}
	}
}

bool UGameLobbyFunctionLibrary::IsLocalPlayer(const UObject* WorldContext, int32 PlayerId)
{
	if (!IsValid(WorldContext))
		return false;

	if (auto PC = WorldContext->GetWorld()->GetFirstPlayerController())
	{
		if (auto PS = PC->GetPlayerState<APlayerState>())
		{
			return PS->GetPlayerId() == PlayerId;
		}
	}

	return false;
}

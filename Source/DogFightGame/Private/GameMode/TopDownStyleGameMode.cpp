#include "GameMode/TopDownStyleGameMode.h"
#include "Common/LuaEventDef.h"
#include "DamageCalculator/DamageCalculatorBase.h"
#include "FunctionLibrary/CommonGameplayFunctionLibrary.h"
#include "GameMode/TopDownStyleGameState.h"
#include "GameMode/GameModeComponent/InGameMessageSenderComponent.h"
#include "GameService/DamageService.h"
#include "GameService/GameService.h"
#include "GameService/LuaEventService.h"
#include "Pawn/PlayerPawn/TopDownStylePlayerPawn.h"
#include "Player/TopDownStylePlayerState.h"
#include "PlayerController/TopDownStylePlayerController.h"

ATopDownStyleGameMode::ATopDownStyleGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create in-game message sender
	InGameMessageSenderComponent = CreateDefaultSubobject<UInGameMessageSenderComponent>("InGameMessageSender");
}

void ATopDownStyleGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATopDownStyleGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Create damage calculator
	if (auto DamageService = UGameService::GetGameService<UDamageService>())
	{
		DamageCalculator = DamageService->CreateDamageCalculator(UDamageCalculatorBase::StaticClass(), DamageCalculatorPath, this);
		// Listen the damage event
		DamageCalculator->OnDamageEventOccured.AddDynamic(this, &ATopDownStyleGameMode::OnDamageEventOccured);
	}
}

void ATopDownStyleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Record new player controller
	if (auto PC = Cast<ATopDownStylePlayerController>(NewPlayer))
	{
		AllPlayerControllers.Add(PC);

		if (auto PS = PC->GetPlayerState<ATopDownStylePlayerState>())
		{
			PS->OnPlayerStateChanged.AddUObject(this, &ATopDownStyleGameMode::OnAnyPlayerStateChanged);
		}
	}
}

void ATopDownStyleGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	// Remove exiting player controller
	if (auto PC = Cast<ATopDownStylePlayerController>(Exiting))
	{
		AllPlayerControllers.Remove(PC);

		if (auto PS = PC->GetPlayerState<ATopDownStylePlayerState>())
		{
			PS->OnPlayerStateChanged.RemoveAll(this);
		}
	}
}

void ATopDownStyleGameMode::PlayerReadyForGame(ATopDownStylePlayerController* InPC)
{
	ReadyPlayerCount++;

	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_ReadyPlayerCount, FString::Printf(TEXT("%d"), ReadyPlayerCount));
	}
}

void ATopDownStyleGameMode::PlayerRequestFinishRound(ATopDownStylePlayerController* InPC)
{
	if (!IsValid(InPC))
	{
		return;
	}

	int32 PlayerId = -1;
	if (auto PS = InPC->GetPlayerState<ATopDownStylePlayerState>())
	{
		PlayerId = PS->GetPlayerId();
	}

	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_FinishPlayerRound, PlayerId);
	}
}

UGameTimelineComponent* ATopDownStyleGameMode::GetGameTimelineComponent() const
{
	if (auto GS = GetGameState<ATopDownStyleGameState>())
	{
		return GS->GetGameTimelineComponent();
	}

	return nullptr;
}

#define FOREACH_PLAYER_CONTROLLER_DO(List, FuncName)		\
	for (auto PC : List) \
	{ \
		if (PC.IsValid()) \
		{ \
			PC->FuncName(); \
		} \
	}

void ATopDownStyleGameMode::SetEnableCharacterMoveForAllPlayers(bool bEnable)
{
	for (auto PC : AllPlayerControllers)
	{
		if (PC.IsValid())
		{
			if (auto PlayerPawn = Cast<ATopDownStylePlayerPawn>(PC->GetPawn()))
			{
				PlayerPawn->ServerSetCharacterMovable(bEnable);
			}
		}
	}
}

void ATopDownStyleGameMode::DamageActor(int32 DamageId, AActor* Target, float BaseDamage, AActor* Causer)
{
	if (!IsValid(DamageCalculator))
	{
		return;
	}

	if (auto DamageService = UGameService::GetGameService<UDamageService>())
	{
		DamageService->ApplyDamageToActor(DamageId, Target, BaseDamage, Causer, DamageCalculator);
	}
}

void ATopDownStyleGameMode::DamageArea(int32 DamageId, const FVector& Origin, float Radius, float BaseDamage,
	AActor* Causer)
{
	if (!IsValid(DamageCalculator))
	{
		return;
	}

	if (auto DamageService = UGameService::GetGameService<UDamageService>())
	{
		TArray<FOverlapResult> OverlapResults;
		FCollisionShape CollisionSphere;
		CollisionSphere.SetSphere(Radius);
		if (GetWorld()->OverlapMultiByChannel(OverlapResults, Origin, FQuat::Identity, ECC_WorldStatic, CollisionSphere))
		{
			TArray<AActor*> AppliedActors;
			// Apply damage to all result
			for (auto& Result : OverlapResults)
			{
				if (auto Actor = Result.GetActor())
				{
					// Do NOT damage same actor multiple times
					if (!AppliedActors.Contains(Actor))
					{
						DamageService->ApplyDamageToActor(DamageId, Actor, BaseDamage, Causer, DamageCalculator);
						AppliedActors.Add(Actor);
					}
				}
			}
		}
	}
}

void ATopDownStyleGameMode::OnDamageEventOccured(UExtendedDamageInstance* DamageInstance,
                                                 const FExtendedDamageEvent& DamageEvent)
{
	if (auto DamageService = UGameService::GetGameService<UDamageService>())
	{
		auto DisplayParams = DamageService->GetDamageDisplayParameters(DamageInstance, DamageEvent);
		// Broadcast to all clients
		UCommonGameplayFunctionLibrary::CreateDamageDisplayByPlayerId(this, DisplayParams, -1);
	}
}

void ATopDownStyleGameMode::OnAnyPlayerStateChanged(ATopDownStylePlayerState* PlayerState,
	ETopDownStylePlayerState::Type InState)
{
	if (!IsValid(PlayerState))
	{
		return;
	}

	auto PlayerId = PlayerState->GetPlayerId();
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		switch(InState)
		{
		case ETopDownStylePlayerState::PS_Alive:
			break;
		case ETopDownStylePlayerState::PS_Dead:
			LuaEventService->SendEventToLua(ELuaEvent::Type::LuaEvent_OnPlayerCharacterDead, PlayerId);
			break;
		case ETopDownStylePlayerState::PS_None:
		default: ;
		}
	}
}

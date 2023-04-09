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
	}
}

void ATopDownStyleGameMode::PlayerReadyForGame(ATopDownStylePlayerController* InPC)
{
	ReadyPlayerCount++;

	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua_OneParam(ELuaEvent::Type::LuaEvent_ReadyPlayerCount, FString::Printf(TEXT("%d"), ReadyPlayerCount));
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

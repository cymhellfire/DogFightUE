#include "PlayerController/TopDownStylePlayerController.h"

#include "EnhancedInputComponent.h"
#include "Card/Card.h"
#include "Common/DogFightGameLog.h"
#include "Common/LuaEventDef.h"
#include "GameMode/TopDownStyleGameMode.h"
#include "GameMode/GameModeComponent/InGameMessageSenderComponent.h"
#include "GameService/DebugPanelService.h"
#include "GameService/GameEffectService.h"
#include "GameService/GameInputService.h"
#include "GameService/GameService.h"
#include "GameService/LuaEventService.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Pawn/PlayerCharacter/TopDownStylePlayerCharacter.h"
#include "Player/TopDownStylePlayerState.h"
#include "Player/ControllerComponent/CardTargetProviderComponent.h"
#include "PlayerController/PlayerControllerComponent/InGameMessageReceiverComponent.h"
#include "PlayerController/PlayerControllerComponent/InGameUIInteractComponent.h"
#include "PlayerController/PlayerControllerComponent/InGameWidgetManipulatorComponent.h"

ATopDownStylePlayerController::ATopDownStylePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create in-game message receiver
	InGameMessageReceiverComponent = CreateDefaultSubobject<UInGameMessageReceiverComponent>("InGameMessageReceiver");
	// Create target acquire component
	CardTargetProviderComponent = CreateDefaultSubobject<UCardTargetProviderComponent>("CardTargetProvider");
	CardTargetProviderComponent->OnCardTargetAcquired.AddDynamic(this, &ATopDownStylePlayerController::OnCardTargetAcquired);
	// Create in-game widget manipulator
	InGameWidgetManipulatorComponent = CreateDefaultSubobject<UInGameWidgetManipulatorComponent>("InGameWidgetManipulator");
	// Create in-game UI interact component
	InGameUIInteractComponent = CreateDefaultSubobject<UInGameUIInteractComponent>("InGameUIInteract");

	bShowMouseCursor = true;
}

void ATopDownStylePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		// ServerMarkPlayerReady();
		GetWorld()->GetTimerManager().SetTimer(RandomTimer, this, &ATopDownStylePlayerController::OnTimerExpired,
			FMath::RandRange(2, 5));
	}
}

void ATopDownStylePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ATopDownStylePlayerController, CharacterPawn, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ATopDownStylePlayerController, ControllingPawn, SharedParams);
}

void ATopDownStylePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	CardTargetProviderComponent->InitializeInput();
	InGameUIInteractComponent->InitializeInput(Cast<UEnhancedInputComponent>(InputComponent));
}

void ATopDownStylePlayerController::ClientAddInputMapping_Implementation(EInputMappingType::Type InputType, EInputMappingPriority::Type Priority)
{
	// Add the input mapping on client side
	if (auto GameInputService = UGameService::GetGameService<UGameInputService>())
	{
		GameInputService->AddInputMapping(InputType, Priority);
	}
}

void ATopDownStylePlayerController::ClientRemoveInputMapping_Implementation(EInputMappingType::Type InputType)
{
	// Remove the input mapping on client side
	if (auto GameInputService = UGameService::GetGameService<UGameInputService>())
	{
		GameInputService->RemoveInputMapping(InputType);
	}
}

void ATopDownStylePlayerController::ServerRequestFinishRound_Implementation()
{
	if (ATopDownStyleGameMode* GameMode = Cast<ATopDownStyleGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->PlayerRequestFinishRound(this);
	}
}

void ATopDownStylePlayerController::ClientSendPlayerRoundStartEvent_Implementation(int32 PlayerId)
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::LuaEvent_StartPlayerRound, PlayerId);
	}
}

void ATopDownStylePlayerController::ClientSendPlayerRoundFinishEvent_Implementation(int32 PlayerId)
{
	if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
	{
		LuaEventService->SendEventToLua(ELuaEvent::LuaEvent_FinishPlayerRound, PlayerId);
	}
}

void ATopDownStylePlayerController::SpawnCharacterPawn()
{
	if (HasAuthority())
	{
		if (CharacterPawn == nullptr)
		{
			MARK_PROPERTY_DIRTY_FROM_NAME(ATopDownStylePlayerController, CharacterPawn, this);

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			CharacterPawn = GetWorld()->SpawnActor<ATopDownStylePlayerCharacter>(CharacterClass, FVector::ZeroVector,
				FRotator::ZeroRotator, SpawnParameters);

			// Establish dependency on character
			if (auto MyPlayerState = GetPlayerState<ATopDownStylePlayerState>())
			{
				MyPlayerState->InitWithCharacter(CharacterPawn);
			}
		}
	}
}

void ATopDownStylePlayerController::ServerSetControllingPawn_Implementation(ATopDownStylePlayerCharacter* InPawn)
{
	if (InPawn == ControllingPawn)
	{
		return;
	}

	auto LastPawn = ControllingPawn;
	MARK_PROPERTY_DIRTY_FROM_NAME(ATopDownStylePlayerController, ControllingPawn, this);
	ControllingPawn = InPawn;
	if (HasAuthority())
	{
		OnRep_ControllingPawn(LastPawn);
	}
}

void ATopDownStylePlayerController::ServerMarkPlayerReady_Implementation()
{
	if (ATopDownStyleGameMode* GameMode = Cast<ATopDownStyleGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->PlayerReadyForGame(this);
	}
}

void ATopDownStylePlayerController::OnTimerExpired()
{
	if (RandomTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(RandomTimer);
	}

	ServerMarkPlayerReady();
}

void ATopDownStylePlayerController::ServerUseCardByInstanceId_Implementation(int32 InId)
{
	// if (auto PS = GetPlayerState<ATopDownStylePlayerState>())
	// {
	// 	// Let player state start the using process
	// 	PS->ServerTryToUseCardByInstanceId(InId);
	// }

	// Get current controlling character
	if (!IsValid(ControllingPawn))
	{
		DFLogE(LogDogFightGame, TEXT("No available controlling pawn now."))
		return;
	}

	ControllingPawn->ServerUseCardByInstanceId(InId);
}

void ATopDownStylePlayerController::ServerSendInGameChatMessage_Implementation(const FInGameChatMessage& InMessage)
{
	if (auto GM = Cast<ATopDownStyleGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (auto MessageSender = GM->GetInGameMessageSender())
		{
			MessageSender->BroadcastInGameChatMessage(InMessage);
		}
	}
}

void ATopDownStylePlayerController::StartAcquireTargets(FTargetAcquireSettings Settings, TFunction<void(bool bSuccess, TArray<FAcquiredTargetInfo>)> Callback)
{
	AcquireTargetCallback = Callback;

	// Notify the client to start acquire target process
	ClientStartAcquireTargets(Settings);
}

void ATopDownStylePlayerController::StartAcquireTargetsWithoutCallback(FTargetAcquireSettings Settings)
{
	// Notify the client to start acquire target process
	ClientStartAcquireTargets(Settings);
}

void ATopDownStylePlayerController::OnCardTargetAcquired(bool bSuccess)
{
	if (bSuccess)
	{
		ServerFinishAcquireTargets(bSuccess, CardTargetProviderComponent->GetLastTargetInfoList());
	}
	else
	{
		ServerFinishAcquireTargets(bSuccess, TArray<FAcquiredTargetInfo>());
	}
}

void ATopDownStylePlayerController::ClientStartAcquireTargets_Implementation(FTargetAcquireSettings Settings)
{
	CardTargetProviderComponent->AcquireTarget(Settings);
}

void ATopDownStylePlayerController::ServerFinishAcquireTargets_Implementation(bool bSuccess, const TArray<FAcquiredTargetInfo>& TargetInfos)
{
	if (AcquireTargetCallback)
	{
		AcquireTargetCallback(bSuccess, TargetInfos);

		// The callback should be clear once invoked
		AcquireTargetCallback.Reset();
	}

	OnTargetAcquired.Broadcast(bSuccess, TargetInfos);
}

void ATopDownStylePlayerController::ToggleDebugPanel()
{
	if (auto GameInstance = Cast<UDogFightGameInstance>(GetGameInstance()))
	{
		if (auto DebugPanelService = Cast<UDebugPanelService>(GameInstance->GetGameService(UDebugPanelService::StaticClass()->GetFName())))
		{
			DebugPanelService->ToggleDebugPanel();
		}
	}
}

void ATopDownStylePlayerController::TestAttackTarget()
{
	if (IsValid(CharacterPawn))
	{
		CharacterPawn->TestAttackTarget();
	}
}

void ATopDownStylePlayerController::OnRep_ControllingPawn(ATopDownStylePlayerCharacter* LastPawn)
{
	DFLog(LogDogFightGame, TEXT("Old %s -> New %s"), (IsValid(LastPawn) ? *LastPawn->GetName() : TEXT("null")),
		(IsValid(ControllingPawn) ? *ControllingPawn->GetName() : TEXT("null")));

	
}

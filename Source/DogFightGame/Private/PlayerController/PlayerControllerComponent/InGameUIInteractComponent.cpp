// Dog Fight Game Code By CYM.

#include "PlayerController/PlayerControllerComponent/InGameUIInteractComponent.h"

#include "EnhancedInputComponent.h"
#include "Common/LuaEventDef.h"
#include "GameService/GameService.h"
#include "GameService/LuaEventService.h"
#include "Pawn/PlayerCharacter/TopDownStylePlayerCharacter.h"


// Sets default values for this component's properties
UInGameUIInteractComponent::UInGameUIInteractComponent()
{
}

void UInGameUIInteractComponent::InitializeInput(UEnhancedInputComponent* InputComponent)
{
	if (!IsValid(InputComponent))
	{
		return;
	}

	InputComponent->BindAction(SelectCharacterAction, ETriggerEvent::Triggered, this, &UInGameUIInteractComponent::OnSelectedCharacter);
}

// Called when the game starts
void UInGameUIInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInGameUIInteractComponent::OnSelectedCharacter()
{
	auto PC = GetTypedOuter<APlayerController>();
	if (!IsValid(PC))
	{
		return;
	}

	// Try to get the hit character
	FHitResult Hit;
	if (PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, false, Hit))
	{
		if (auto Character = Cast<ATopDownStylePlayerCharacter>(Hit.GetActor()))
		{
			if (auto LuaEventService = UGameService::GetGameService<ULuaEventService>())
			{
				float MouseX, MouseY;
				PC->GetMousePosition(MouseX, MouseY);
				LuaEventService->SendEventToLua(ELuaEvent::LuaEvent_ShowWidget, "CharacterMenu", Character, FVector2D(MouseX, MouseY));
			}
		}
	}
}

#include "PlayerController/PlayerControllerComponent/InGameWidgetManipulatorComponent.h"

#include "GameService/DamageService.h"
#include "GameService/GameService.h"
#include "GameService/GameWidgetService.h"

UInGameWidgetManipulatorComponent::UInGameWidgetManipulatorComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UInGameWidgetManipulatorComponent::ClientAddInGameWidget_Implementation(const FString& InWidgetName, bool bSilent)
{
	if (auto WidgetService = UGameService::GetGameService<UGameWidgetService>())
	{
		WidgetService->AddWidget(InWidgetName, bSilent);
	}
}

void UInGameWidgetManipulatorComponent::ClientShowInGameWidget_Implementation(const FString& InWidgetName, bool bCreateIfNoWidget)
{
	if (auto WidgetService = UGameService::GetGameService<UGameWidgetService>())
	{
		WidgetService->ShowWidget(InWidgetName, bCreateIfNoWidget);
	}
}

void UInGameWidgetManipulatorComponent::ClientRemoveInGameWidget_Implementation(const FString& InWidgetName)
{
	if (auto WidgetService = UGameService::GetGameService<UGameWidgetService>())
	{
		WidgetService->RemoveWidget(InWidgetName);
	}
}

void UInGameWidgetManipulatorComponent::ClientHideInGameWidget_Implementation(const FString& InWidgetName)
{
	if (auto WidgetService = UGameService::GetGameService<UGameWidgetService>())
	{
		WidgetService->HideWidget(InWidgetName);
	}
}

void UInGameWidgetManipulatorComponent::ClientShowDamageWidget_Implementation(const FDamageDisplayParams& DamageDisplayParams)
{
	// Create damage display locally
	if (auto DamageService = UGameService::GetGameService<UDamageService>())
	{
		DamageService->CreateDamageDisplay(DamageDisplayParams);
	}
}

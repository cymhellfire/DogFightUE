#include "PlayerController/PlayerControllerComponent/InGameWidgetManipulatorComponent.h"

#include "GameService/DamageService.h"
#include "GameService/GameService.h"
#include "GameService/GameWidgetService.h"

UInGameWidgetManipulatorComponent::UInGameWidgetManipulatorComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UInGameWidgetManipulatorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ProcessDamageDisplayQueue();
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
		if (!DamageService->CreateDamageDisplay(DamageDisplayParams))
		{
			// Push damage info into queue.
			DamageDisplayQueue.Push(DamageDisplayParams);
		}
	}
}

void UInGameWidgetManipulatorComponent::ProcessDamageDisplayQueue()
{
	if (DamageDisplayQueue.IsEmpty())
	{
		return;
	}

	TArray<int32> RemoveIndex;
	for (int32 i = 0; i < DamageDisplayQueue.Num(); ++i)
	{
		if (auto DamageService = UGameService::GetGameService<UDamageService>())
		{
			if (DamageService->CreateDamageDisplay(DamageDisplayQueue[i]))
			{
				// Record all damage display succeed
				RemoveIndex.Add(i);
			}
		}
	}

	// Clear the damage info
	for (int32 i = RemoveIndex.Num() - 1; i >=0; --i)
	{
		DamageDisplayQueue.RemoveAt(RemoveIndex[i]);
	}
}

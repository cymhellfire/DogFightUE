#include "GameObject/Component/WidgetLocatorComponent.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "DamageReceiver/DamageReceiverComponent.h"
#include "Interface/DamageReceiverActorInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InGame/DamageDisplayWidget.h"

UWidgetLocatorComponent::UWidgetLocatorComponent()
{
	// Enable component tick
	PrimaryComponentTick.bCanEverTick = true;
}

void UWidgetLocatorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update widget position on screen
	UpdateWidgetPos();
}

void UWidgetLocatorComponent::SetWidget(UUserWidget* InWidget)
{
	if (IsValid(InWidget))
	{
		DisplayWidget = InWidget;
	}
}

void UWidgetLocatorComponent::UpdateWidgetPos()
{
	if (!DisplayWidget.IsValid())
	{
		return;
	}

	auto WorldPos = GetOwner()->GetActorLocation();
	FVector2D ScreenPos;
	if (UGameplayStatics::ProjectWorldToScreen(GEngine->GetFirstLocalPlayerController(GetWorld()), WorldPos, ScreenPos))
	{
		// Apply offset
		ScreenPos += WidgetOffset;
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		if (auto Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(DisplayWidget.Get()))
		{
			Slot->SetPosition(ScreenPos / Scale);
		}
	}
}

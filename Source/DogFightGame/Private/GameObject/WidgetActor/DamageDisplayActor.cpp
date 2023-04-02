#include "GameObject/WidgetActor/DamageDisplayActor.h"

#include "GameObject/Component/WidgetLocatorComponent.h"
#include "UI/InGame/DamageDisplayWidget.h"

ADamageDisplayActor::ADamageDisplayActor()
{
	// Create root component
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	// Create locator component
	LocatorComponent = CreateDefaultSubobject<UWidgetLocatorComponent>("LocatorComponent");
}

void ADamageDisplayActor::SetWidget(UDamageDisplayWidget* InWidget)
{
	if (!IsValid(InWidget))
	{
		return;
	}

	DamageDisplayWidget = InWidget;
	LocatorComponent->SetWidget(InWidget);
}

void ADamageDisplayActor::SetDamageInfo(const FDamageDisplayParams& DisplayParams)
{
	if (IsValid(DamageDisplayWidget))
	{
		DamageDisplayWidget->AddDamageItem(DisplayParams);
	}
}

void ADamageDisplayActor::Activate(float Lifetime)
{
	if (Lifetime > 0.f)
	{
		SetLifeSpan(Lifetime);
	}

	// Notify the widget
	if (IsValid(DamageDisplayWidget))
	{
		DamageDisplayWidget->OnOwnerActivated();
	}
}

void ADamageDisplayActor::LifeSpanExpired()
{
	Dead();
}

void ADamageDisplayActor::Dead()
{
	// Notify the widget
	if (IsValid(DamageDisplayWidget))
	{
		DamageDisplayWidget->OnOwnerDead();
	}

	// Broadcast event
	OnDamageDisplayActorDead.Broadcast(this);
}

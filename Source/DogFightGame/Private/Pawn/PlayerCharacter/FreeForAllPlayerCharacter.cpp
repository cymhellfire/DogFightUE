#include "Pawn/PlayerCharacter/FreeForAllPlayerCharacter.h"
#include "UI/InGame/PlayerCharacterStateWidget.h"

AFreeForAllPlayerCharacter::AFreeForAllPlayerCharacter()
{
	// Create components
	DamageReceiverComponent = CreateDefaultSubobject<UDamageReceiverComponent>("DamageReceiverComponent");
	StateWidgetLocatorComponent = CreateDefaultSubobject<UWidgetLocatorComponent>("StateWidgetLocatorComponent");
}

void AFreeForAllPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeStateWidget();

	DamageReceiverComponent->OnHealthChanged.AddDynamic(this, &AFreeForAllPlayerCharacter::OnHealthChanged);
}

void AFreeForAllPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	DeinitializeStateWidget();

	DamageReceiverComponent->OnHealthChanged.RemoveDynamic(this, &AFreeForAllPlayerCharacter::OnHealthChanged);
}

FVector AFreeForAllPlayerCharacter::GetProjectileSpawnLocation() const
{
	auto Offset = GetTransform().TransformVector(ProjectileSpawnOffset);
	return GetActorLocation() + Offset;
}

void AFreeForAllPlayerCharacter::InitializeStateWidget()
{
	if (StateWidgetClass.IsNull())
	{
		return;
	}

	auto WidgetClass = StateWidgetClass.IsValid() ? StateWidgetClass.Get() : StateWidgetClass.LoadSynchronous();
	if (WidgetClass)
	{
		StateWidget = Cast<UPlayerCharacterStateWidget>(CreateWidget(GetWorld(), WidgetClass));

		// Active widget
		if (IsValid(StateWidget))
		{
			StateWidgetLocatorComponent->SetWidget(StateWidget);
			StateWidget->OnActive();
		}
	}
}

void AFreeForAllPlayerCharacter::DeinitializeStateWidget()
{
	if (IsValid(StateWidget))
	{
		StateWidget->RemoveFromParent();
		StateWidget->ConditionalBeginDestroy();
	}
}

void AFreeForAllPlayerCharacter::OnHealthChanged(float CurHealth, float MaxHealth)
{
	if (IsValid(StateWidget))
	{
		StateWidget->OnHealthChanged(CurHealth, MaxHealth);
	}
}

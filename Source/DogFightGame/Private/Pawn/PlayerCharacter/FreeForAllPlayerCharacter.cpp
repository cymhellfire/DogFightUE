#include "Pawn/PlayerCharacter/FreeForAllPlayerCharacter.h"

#include "Pawn/PlayerCharacter/RagdollComponent.h"
#include "UI/InGame/PlayerCharacterStateWidget.h"

AFreeForAllPlayerCharacter::AFreeForAllPlayerCharacter()
{
	// Create components
	DamageReceiverComponent = CreateDefaultSubobject<UDamageReceiverComponent>("DamageReceiverComponent");
	StateWidgetLocatorComponent = CreateDefaultSubobject<UWidgetLocatorComponent>("StateWidgetLocatorComponent");
	RagdollComponent = CreateDefaultSubobject<URagdollComponent>("RagdollComponent");

	// Initial value
	bAlive = true;
}

void AFreeForAllPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeStateWidget();

	DamageReceiverComponent->OnHealthChanged.AddDynamic(this, &AFreeForAllPlayerCharacter::OnHealthChanged);
	DamageReceiverComponent->OnNoHealth.AddDynamic(this, &AFreeForAllPlayerCharacter::OnNoHealth);
}

void AFreeForAllPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	DeinitializeStateWidget();

	DamageReceiverComponent->OnHealthChanged.RemoveDynamic(this, &AFreeForAllPlayerCharacter::OnHealthChanged);
	DamageReceiverComponent->OnNoHealth.RemoveDynamic(this, &AFreeForAllPlayerCharacter::OnNoHealth);
}

FVector AFreeForAllPlayerCharacter::GetProjectileSpawnLocation() const
{
	auto Offset = GetTransform().TransformVector(ProjectileSpawnOffset);
	return GetActorLocation() + Offset;
}

void AFreeForAllPlayerCharacter::SetRagdollEnabled(bool bEnable)
{
	RagdollComponent->ServerSetRagdollActive(bEnable);
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

void AFreeForAllPlayerCharacter::Dead()
{
	if (!bAlive)
	{
		return;
	}

	// Mark as dead
	bAlive = false;

	// Enable ragdoll when dead
	RagdollComponent->ServerSetRagdollActive(true);
}

void AFreeForAllPlayerCharacter::OnHealthChanged(float CurHealth, float MaxHealth)
{
	if (IsValid(StateWidget))
	{
		StateWidget->OnHealthChanged(CurHealth, MaxHealth);
	}
}

void AFreeForAllPlayerCharacter::OnNoHealth()
{
	if (!HasAuthority())
	{
		return;
	}

	Dead();
}

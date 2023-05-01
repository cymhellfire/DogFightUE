#include "Pawn/PlayerCharacter/TopDownStylePlayerCharacter.h"

#include "AIController.h"
#include "GameObject/Buff/NewBuffBase.h"
#include "Pawn/PlayerCharacter/BuffManagerComponent.h"
#include "Pawn/PlayerCharacter/RagdollComponent.h"
#include "UI/InGame/PlayerCharacterStateWidget.h"

ATopDownStylePlayerCharacter::ATopDownStylePlayerCharacter()
{
	// Create components
	DamageReceiverComponent = CreateDefaultSubobject<UDamageReceiverComponent>("DamageReceiverComponent");
	StateWidgetLocatorComponent = CreateDefaultSubobject<UWidgetLocatorComponent>("StateWidgetLocatorComponent");
	RagdollComponent = CreateDefaultSubobject<URagdollComponent>("RagdollComponent");
	BuffManagerComponent = CreateDefaultSubobject<UBuffManagerComponent>("BuffManagerComponent");

	// Initial value
	bAlive = true;
}

void ATopDownStylePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeStateWidget();

	DamageReceiverComponent->OnHealthChanged.AddDynamic(this, &ATopDownStylePlayerCharacter::OnHealthChanged);
	DamageReceiverComponent->OnNoHealth.AddDynamic(this, &ATopDownStylePlayerCharacter::OnNoHealth);

	if (auto AIController = Cast<AAIController>(GetController()))
	{
		PathFollowingComponent = AIController->GetPathFollowingComponent();
		if (PathFollowingComponent.IsValid())
		{
			PathFollowingComponent->OnRequestFinished.AddUObject(this, &ATopDownStylePlayerCharacter::OnMoveFinished);
		}
	}
}

void ATopDownStylePlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	DeinitializeStateWidget();

	DamageReceiverComponent->OnHealthChanged.RemoveDynamic(this, &ATopDownStylePlayerCharacter::OnHealthChanged);
	DamageReceiverComponent->OnNoHealth.RemoveDynamic(this, &ATopDownStylePlayerCharacter::OnNoHealth);

	if (PathFollowingComponent.IsValid())
	{
		PathFollowingComponent->OnRequestFinished.RemoveAll(this);
	}
}

FVector ATopDownStylePlayerCharacter::GetProjectileSpawnLocation() const
{
	auto Offset = GetTransform().TransformVector(ProjectileSpawnOffset);
	return GetActorLocation() + Offset;
}

void ATopDownStylePlayerCharacter::SetRagdollEnabled(bool bEnable)
{
	RagdollComponent->ServerSetRagdollActive(bEnable);
}

void ATopDownStylePlayerCharacter::InitializeStateWidget()
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

void ATopDownStylePlayerCharacter::DeinitializeStateWidget()
{
	if (IsValid(StateWidget))
	{
		StateWidget->RemoveFromParent();
		StateWidget->ConditionalBeginDestroy();
	}
}

void ATopDownStylePlayerCharacter::Dead()
{
	if (!bAlive)
	{
		return;
	}

	// Mark as dead
	bAlive = false;

	// Enable ragdoll when dead
	RagdollComponent->ServerSetRagdollActive(true);

	// Trigger delegate
	OnCharacterDead.Broadcast(this);
}

void ATopDownStylePlayerCharacter::OnMoveFinished(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		OnCharacterMoveFinished.Broadcast();
	}
}

void ATopDownStylePlayerCharacter::OnHealthChanged(float CurHealth, float MaxHealth)
{
	if (IsValid(StateWidget))
	{
		StateWidget->OnHealthChanged(CurHealth, MaxHealth);
	}
}

void ATopDownStylePlayerCharacter::OnNoHealth()
{
	if (!HasAuthority())
	{
		return;
	}

	Dead();
}

void ATopDownStylePlayerCharacter::AddBuff(UNewBuffBase* InBuff)
{
	if (IsValid(BuffManagerComponent))
	{
		BuffManagerComponent->AddBuff(InBuff);
	}
}

void ATopDownStylePlayerCharacter::RemoveBuff(UNewBuffBase* InBuff)
{
	if (IsValid(BuffManagerComponent))
	{
		BuffManagerComponent->RemoveBuff(InBuff);
	}
}

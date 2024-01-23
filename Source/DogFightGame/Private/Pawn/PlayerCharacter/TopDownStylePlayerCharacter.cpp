#include "Pawn/PlayerCharacter/TopDownStylePlayerCharacter.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "MotionWarpingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Common/WeaponCommon.h"
#include "Controller/ActionCharacterAIController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameObject/Buff/NewBuffBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "Pawn/PlayerCharacter/ArsenalComponent.h"
#include "Pawn/PlayerCharacter/BuffManagerComponent.h"
#include "Pawn/PlayerCharacter/CharacterAnimComponent.h"
#include "Pawn/PlayerCharacter/RagdollComponent.h"
#include "UI/InGame/PlayerCharacterStateWidget.h"

ATopDownStylePlayerCharacter::ATopDownStylePlayerCharacter()
{
	// Create components
	DamageReceiverComponent = CreateDefaultSubobject<UDamageReceiverComponent>("DamageReceiverComponent");
	StateWidgetLocatorComponent = CreateDefaultSubobject<UWidgetLocatorComponent>("StateWidgetLocatorComponent");
	RagdollComponent = CreateDefaultSubobject<URagdollComponent>("RagdollComponent");
	BuffManagerComponent = CreateDefaultSubobject<UBuffManagerComponent>("BuffManagerComponent");
	ArsenalComponent = CreateDefaultSubobject<UArsenalComponent>("ArsenalComponent");
	AnimComponent = CreateDefaultSubobject<UCharacterAnimComponent>("CharacterAnimComponent");
	MotionWarpComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");

	// Initial value
	bAlive = true;
	AIControllerClass = AActionCharacterAIController::StaticClass();
}

void ATopDownStylePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeStateWidget();

	DamageReceiverComponent->OnHealthChanged.AddDynamic(this, &ATopDownStylePlayerCharacter::OnHealthChanged);
	DamageReceiverComponent->OnNoHealth.AddDynamic(this, &ATopDownStylePlayerCharacter::OnNoHealth);

	if (auto AIController = Cast<AActionCharacterAIController>(GetController()))
	{
		if (auto PathFollowingComponent = AIController->GetPathFollowingComponent())
		{
			PathFollowingComponent->OnRequestFinished.AddUObject(this, &ATopDownStylePlayerCharacter::OnMoveFinished);
		}

		AIController->OnReachStopDistance.AddUObject(this, &ATopDownStylePlayerCharacter::OnReachStopDistance);
	}

	// Init test weapon
	if (WeaponData.IsValid() && IsValid(ArsenalComponent))
	{
		ArsenalComponent->EquipWeapon(EWeaponSlotType::WST_Primary, WeaponData);
	}
}

void ATopDownStylePlayerCharacter::TestAttackTarget()
{
	for (TActorIterator<ATopDownStylePlayerCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		if (*Iter == this)
		{
			continue;
		}

		// Take target
		if (IsValid(ArsenalComponent))
		{
			ArsenalComponent->AttackTarget(FWeaponActionTarget(*Iter));
		}
	}
}

void ATopDownStylePlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	DeinitializeStateWidget();

	DamageReceiverComponent->OnHealthChanged.RemoveDynamic(this, &ATopDownStylePlayerCharacter::OnHealthChanged);
	DamageReceiverComponent->OnNoHealth.RemoveDynamic(this, &ATopDownStylePlayerCharacter::OnNoHealth);

	if (auto AIController = Cast<AActionCharacterAIController>(GetController()))
	{
		if (auto PathFollowingComponent = AIController->GetPathFollowingComponent())
		{
			PathFollowingComponent->OnRequestFinished.RemoveAll(this);
		}

		AIController->OnReachStopDistance.RemoveAll(this);
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

float ATopDownStylePlayerCharacter::PlayActionAnimation(UAnimMontage* InMontage)
{
	if (IsValid(AnimComponent))
	{
		return AnimComponent->PlayAnimation(InMontage);
	}
	return IActionCharacterInterface::PlayActionAnimation(InMontage);
}

float ATopDownStylePlayerCharacter::PlayActionAnimationWithWarping(UAnimMontage* InMontage, FName TargetName,
	const FVector& TargetPos)
{
	if (IsValid(AnimComponent))
	{
		return AnimComponent->PlayAnimationWithWarping(InMontage, TargetName, TargetPos);
	}
	return IActionCharacterInterface::PlayActionAnimationWithWarping(InMontage, TargetName, TargetPos);
}

void ATopDownStylePlayerCharacter::MoveToTarget(const FVector& Target, float StopDistance)
{
	if (auto AIController = Cast<AActionCharacterAIController>(GetController()))
	{
		AIController->MoveToTargetWithStopDistance(Target, StopDistance);
	}
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

void ATopDownStylePlayerCharacter::OnReachStopDistance()
{
	StopMoveImmediately();
	OnReachActionDistance.Broadcast();
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

void ATopDownStylePlayerCharacter::StopMoveImmediately()
{
	if (auto MovementComponent = GetMovementComponent())
	{
		MovementComponent->StopMovementImmediately();
	}
}

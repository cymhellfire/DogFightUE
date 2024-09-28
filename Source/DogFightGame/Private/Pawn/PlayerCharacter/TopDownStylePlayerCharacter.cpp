#include "Pawn/PlayerCharacter/TopDownStylePlayerCharacter.h"

#include "EngineUtils.h"
#include "MotionWarpingComponent.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Common/WeaponCommon.h"
#include "FunctionLibrary/AvatarFunctionLibrary.h"
#include "GameObject/Buff/NewBuffBase.h"
#include "Pawn/PlayerCharacter/ArsenalComponent.h"
#include "Pawn/PlayerCharacter/BuffManagerComponent.h"
#include "Pawn/PlayerCharacter/RagdollComponent.h"
#include "UI/InGame/PlayerCharacterStateWidget.h"
#include "Net/UnrealNetwork.h"
#include "Pawn/PlayerCharacter/CharacterInventoryComponent.h"

ATopDownStylePlayerCharacter::ATopDownStylePlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create components
	DamageReceiverComponent = CreateDefaultSubobject<UDamageReceiverComponent>("DamageReceiverComponent");
	StateWidgetLocatorComponent = CreateDefaultSubobject<UWidgetLocatorComponent>("StateWidgetLocatorComponent");
	RagdollComponent = CreateDefaultSubobject<URagdollComponent>("RagdollComponent");
	BuffManagerComponent = CreateDefaultSubobject<UBuffManagerComponent>("BuffManagerComponent");
	ArsenalComponent = CreateDefaultSubobject<UArsenalComponent>("ArsenalComponent");
	MotionWarpComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
	InventoryComponent = CreateDefaultSubobject<UCharacterInventoryComponent>("InventoryComponent");

	// Initial value
	bAlive = true;
}

void ATopDownStylePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeStateWidget();

	DamageReceiverComponent->OnHealthChanged.AddDynamic(this, &ATopDownStylePlayerCharacter::OnHealthChanged);
	DamageReceiverComponent->OnNoHealth.AddDynamic(this, &ATopDownStylePlayerCharacter::OnNoHealth);

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
}

void ATopDownStylePlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ATopDownStylePlayerCharacter, AvatarId, SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(ATopDownStylePlayerCharacter, PlayerId, SharedParams)
}

void ATopDownStylePlayerCharacter::SetPlayerId(int32 InId)
{
	if (!HasAuthority())
		return;

	if (PlayerId == InId)
		return;

	MARK_PROPERTY_DIRTY_FROM_NAME(ATopDownStylePlayerCharacter, PlayerId, this);
	PlayerId = InId;
}

void ATopDownStylePlayerCharacter::ServerSetupAvatarId_Implementation(int32 InId)
{
	if (!HasAuthority())
		return;

	// Skip if given id is the same
	if (AvatarId == InId)
		return;

	MARK_PROPERTY_DIRTY_FROM_NAME(ATopDownStylePlayerCharacter, AvatarId, this);
	AvatarId = InId;

	// Trigger the event on server side
	OnRep_AvatarId();
}

void ATopDownStylePlayerCharacter::OnRep_AvatarId()
{
	UAvatarFunctionLibrary::InitAvatarAppearanceWithConfigId(this, AvatarId);
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

void ATopDownStylePlayerCharacter::ServerUseCardByInstanceId_Implementation(int32 InId)
{
	if (IsValid(InventoryComponent))
	{
		InventoryComponent->ServerTryToUseCardByInstanceId(InId);
	}
}

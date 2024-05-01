// Dog Fight Game Code By CYM.


#include "Pawn/PlayerCharacter/ArsenalComponent.h"

#include "Common/DogFightGameLog.h"
#include "DataAsset/WeaponDataAsset.h"
#include "Engine/AssetManager.h"
#include "Object/WeaponBase.h"
#include "Interface/ActionCharacterInterface.h"


// Sets default values for this component's properties
UArsenalComponent::UArsenalComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UArsenalComponent::EquipWeapon(EWeaponSlotType Slot, const FPrimaryAssetId& InWeaponAsset)
{
	if (!InWeaponAsset.IsValid())
	{
		return false;
	}

	// Get weapon data asset
	if (auto AssetManager = UAssetManager::GetIfValid())
	{
		AssetManager->LoadPrimaryAsset(InWeaponAsset, TArray<FName>(), FStreamableDelegate::CreateLambda([Slot, &InWeaponAsset, this]()
		{
			FAssetData AssetData;
			if (UAssetManager::Get().GetPrimaryAssetData(InWeaponAsset, AssetData))
			{
				if (auto WeaponData = Cast<UWeaponDataAsset>(AssetData.GetAsset()))
				{
					OnWeaponDataLoaded(Slot, WeaponData);
				}
			}
		}));
	}
	return true;
}

void UArsenalComponent::AttackTarget(const FWeaponActionTarget& InTarget, EWeaponSlotType InSlot)
{
	if (auto WeaponPtr = WeaponSlotMap.Find(InSlot))
	{
		auto Weapon = *WeaponPtr;
		Weapon->EnqueueInput(EWeaponActionInput::WAI_Attack, InTarget);
		Weapon->EnqueueInput(EWeaponActionInput::WAI_Finish);
		Weapon->StartInputQueue();
	}
}

// Called when the game starts
void UArsenalComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UArsenalComponent::OnWeaponDataLoaded(EWeaponSlotType Slot, UWeaponDataAsset* InWeaponData)
{
	if (!IsValid(InWeaponData))
	{
		return;
	}

	auto NewWeapon = NewObject<UWeaponBase>(this, InWeaponData->WeaponClass.LoadSynchronous());
	if (IsValid(NewWeapon))
	{
		NewWeapon->SetOwner(Cast<IActionCharacterInterface>(GetOuter()));
		NewWeapon->InitWithWeaponData(InWeaponData);
		WeaponSlotMap.Add(Slot, NewWeapon);

		// Listen key event
		NewWeapon->OnWeaponInputFinished.AddLambda([Slot, this](UWeaponBase* InWeapon)
		{
			if (IsValid(this) && IsValid(InWeapon))
			{
				// Verify the weapon instance
				if (auto WeaponPtr = WeaponSlotMap.Find(Slot))
				{
					if (*WeaponPtr == InWeapon)
					{
#if UE_BUILD_DEVELOPMENT
						static UEnum* SlotEnum = StaticEnum<EWeaponSlotType>();
						UE_LOG(LogDogFightGame, Log, TEXT("[ArsenalComponent] Weapon %s at slot %s finished."), *InWeapon->GetName(), *SlotEnum->GetNameStringByIndex((int32)Slot))
#endif
						OnSlotWeaponFinished.Broadcast(Slot);
					}
				}
			}
		});
	}
}

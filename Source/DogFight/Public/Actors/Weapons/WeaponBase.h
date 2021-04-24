// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DogFight.h"
#include "WeaponCommon.h"
#include "WeaponBase.generated.h"

class UWeaponActionBase;
class AWeaponMeshActor;

enum EWeaponState
{
	WS_None,
	WS_Equipping,
	WS_UnEquipping,
	WS_InAction,
};

/**
 * Struct stores weapon mesh actors and parent socket.
 */
USTRUCT(BlueprintType)
struct FWeaponMeshSetting
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponMeshSetting")
	TSubclassOf<AWeaponMeshActor> WeaponMeshActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponMeshSetting")
	FName ParentSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponMeshSetting")
	float SpawnDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponMeshSetting")
	float DestroyDelay;
};

struct FWeaponSpawningInfo
{
	TSubclassOf<AWeaponMeshActor> WeaponMeshClass;

	FName TargetSocketName;

	float SpawnCountdown;

	float DestroyDelay;

	FWeaponSpawningInfo(FWeaponMeshSetting Settings)
	{
		WeaponMeshClass = Settings.WeaponMeshActorClass;
		TargetSocketName = Settings.ParentSocketName;
		SpawnCountdown = Settings.SpawnDelay;
		DestroyDelay = Settings.DestroyDelay;
	}
};

struct FWeaponDestroyInfo
{
	AWeaponMeshActor* WeaponActor;

	float DestroyCountdown;
};

UCLASS(Blueprintable)
class DOGFIGHT_API UWeaponBase : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UWeaponBase();

	virtual void SetWeaponOwner(ACharacter* NewOwner);

	virtual void Equip();

	virtual void UnEquip();

	virtual void EnqueueWeaponInput(EWeaponActionInput NewInput);

	virtual EWeaponActionInput DequeueWeaponInput();

	/** Reset weapon to the initial action. */
	virtual void ResetWeaponAction();

	EWeaponType GetWeaponType() const { return WeaponType; }

	ACharacter* GetWeaponOwnerCharacter() const { return OwnerCharacter; }

	AWeaponMeshActor* GetWeaponMeshByParentSocket(FName SocketName);

#pragma region FTickableGameObject
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return true; }
	virtual TStatId GetStatId() const override { return Super::GetStatID(); }
#pragma endregion

protected:
	void InitialWeaponActions();

	void UpdateSpawningQueue(float DeltaTime);
	void UpdateDestroyQueue(float DeltaTime);

	void SpawnWeaponActor(FWeaponSpawningInfo SpawnSettings);

	void StartWeaponAction(UWeaponActionBase* ActionToStart);

	void ConsumeNextInput();

	UFUNCTION()
	void OnWeaponEquipped();

	UFUNCTION()
	void OnWeaponUnEquipped();

	UFUNCTION()
	void OnWeaponActionFinished();

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponDelegateNoArgument);
	FWeaponDelegateNoArgument OnWeaponEquippedEvent;

	FWeaponDelegateNoArgument OnWeaponUnEquippedEvent;

	FWeaponDelegateNoArgument OnWeaponActionFinishedEvent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TArray<FWeaponMeshSetting> MeshSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TArray<TSubclassOf<UWeaponActionBase>> WeaponActionClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName InitialActionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UAnimMontage* EquipAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UAnimMontage* UnEquipAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	uint8 bAutoConsumeInput : 1;

	uint8 bEquipped : 1;

	EWeaponState CurrentWeaponState;

	TArray<FWeaponSpawningInfo> SpawningInfos;
	TArray<FWeaponDestroyInfo> DestroyInfos;

	TMap<FName, AWeaponMeshActor*> WeaponMeshActorMap;

	ACharacter* OwnerCharacter;

	FTimerHandle WeaponEquipTimerHandle;
	FTimerHandle WeaponUnEquipTimerHandle;

	UWeaponActionBase* CurrentAction;
	TQueue<EWeaponActionInput> WeaponInputQueue;

	UPROPERTY()
	TMap<FName, UWeaponActionBase*> WeaponActionMap;
};

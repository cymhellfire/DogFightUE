#pragma once

#include "DamageReceiver/DamageReceiverComponent.h"
#include "GameObject/Component/WidgetLocatorComponent.h"
#include "Interface/DamageReceiverActorInterface.h"
#include "Pawn/ActionGameCharacter.h"
#include "TopDownStylePlayerCharacter.generated.h"

class UMotionWarpingComponent;
class ATopDownStylePlayerCharacter;
class UPlayerCharacterStateWidget;
class URagdollComponent;
class UPathFollowingComponent;
class UNewBuffBase;
class UBuffManagerComponent;
class UGameplayAttributesComponent;
class UArsenalComponent;
class UCharacterAnimComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTopDownStylePlayerCharacterDeadEvent, ATopDownStylePlayerCharacter*, Character);

UCLASS()
class DOGFIGHTGAME_API ATopDownStylePlayerCharacter : public AActionGameCharacter, public IDamageReceiverActorInterface
{
	GENERATED_BODY()
public:
	ATopDownStylePlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetPlayerId(int32 InId)
	{
		PlayerId = InId;
	}

	UFUNCTION(Server, Reliable)
	void ServerSetupAvatarId(int32 InId);

	UFUNCTION(BlueprintCallable)
	FVector GetProjectileSpawnLocation() const;

	UFUNCTION(BlueprintCallable)
	void SetRagdollEnabled(bool bEnable);

	UFUNCTION(BlueprintCallable, Category="TopDownStylePlayerCharacter")
	void AddBuff(UNewBuffBase* InBuff);

	UFUNCTION(BlueprintCallable, Category="TopDownStylePlayerCharacter")
	void RemoveBuff(UNewBuffBase* InBuff);

	UFUNCTION(BlueprintCallable, Category="TopDownStylePlayerCharacter")
	UArsenalComponent* GetArsenalComponent() const
	{
		return ArsenalComponent;
	}

	// DamageReceiverActorInterface
	virtual UDamageReceiverComponent* GetDamageReceiverComponent() override
	{
		return DamageReceiverComponent;
	}

	void TestAttackTarget();

protected:
	void InitializeStateWidget();
	void DeinitializeStateWidget();

	virtual void Dead();

	UFUNCTION()
	void OnHealthChanged(float CurHealth, float MaxHealth);

	UFUNCTION()
	void OnNoHealth();

	UFUNCTION()
	void OnRep_AvatarId();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector ProjectileSpawnOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowedTypes="WeaponDataAsset"))
	FPrimaryAssetId WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UPlayerCharacterStateWidget> StateWidgetClass;

	UPROPERTY(BlueprintAssignable, Category="TopDownStylePlayerCharacter")
	FTopDownStylePlayerCharacterDeadEvent OnCharacterDead;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TopDownStylePlayerCharacter")
	UDamageReceiverComponent* DamageReceiverComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TopDownStylePlayerCharacter")
	UWidgetLocatorComponent* StateWidgetLocatorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TopDownStylePlayerCharacter")
	URagdollComponent* RagdollComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TopDownStylePlayerCharacter")
	UBuffManagerComponent* BuffManagerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TopDownStylePlayerCharacter")
	UGameplayAttributesComponent* GameplayAttributesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TopDownStylePlayerCharacter")
	UArsenalComponent* ArsenalComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TopDownStylePlayerCharacter")
	UMotionWarpingComponent* MotionWarpComponent;

	UPROPERTY(Transient)
	UPlayerCharacterStateWidget* StateWidget;

private:

	uint8 bAlive : 1;

	int32 PlayerId;

	UPROPERTY(ReplicatedUsing=OnRep_AvatarId)
	int32 AvatarId;
};

#pragma once

#include "AITypes.h"
#include "DamageReceiver/DamageReceiverComponent.h"
#include "GameFramework/Character.h"
#include "GameObject/Component/WidgetLocatorComponent.h"
#include "Interface/ActionCharacterInterface.h"
#include "Interface/DamageReceiverActorInterface.h"
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
struct FPathFollowingResult;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTopDownStylePlayerCharacterDeadEvent, ATopDownStylePlayerCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTopDownStylePlayerCharacterMoveFinishedEvent);

UCLASS()
class DOGFIGHTGAME_API ATopDownStylePlayerCharacter : public ACharacter, public IDamageReceiverActorInterface, public IActionCharacterInterface
{
	GENERATED_BODY()
public:
	ATopDownStylePlayerCharacter();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SetPlayerId(int32 InId)
	{
		PlayerId = InId;
	}

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

	void StopMoveImmediately();

#pragma region IActionCharacterInterface
	virtual float PlayActionAnimation(UAnimMontage* InMontage) override;
	virtual float PlayActionAnimationWithWarping(UAnimMontage* InMontage, FName TargetName, const FVector& TargetPos) override;
	virtual void MoveToTarget(const FVector& Target, float StopDistance) override;
#pragma endregion IActionCharacterInterface

	void TestAttackTarget();

protected:
	void InitializeStateWidget();
	void DeinitializeStateWidget();

	virtual void Dead();

	void OnMoveFinished(FAIRequestID RequestID, const FPathFollowingResult& Result);

	UFUNCTION()
	void OnHealthChanged(float CurHealth, float MaxHealth);

	UFUNCTION()
	void OnNoHealth();

	virtual void OnReachStopDistance();

#pragma region IActionCharacterInterface
	virtual ACharacter* GetCharacter() override
	{
		return this;
	}
#pragma endregion IActionCharacterInterface

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector ProjectileSpawnOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowedTypes="WeaponDataAsset"))
	FPrimaryAssetId WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UPlayerCharacterStateWidget> StateWidgetClass;

	UPROPERTY(BlueprintAssignable, Category="TopDownStylePlayerCharacter")
	FTopDownStylePlayerCharacterDeadEvent OnCharacterDead;

	UPROPERTY(BlueprintAssignable, Category="TopDownStylePlayerCharacter")
	FTopDownStylePlayerCharacterMoveFinishedEvent OnCharacterMoveFinished;

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
	UCharacterAnimComponent* AnimComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TopDownStylePlayerCharacter")
	UMotionWarpingComponent* MotionWarpComponent;

	UPROPERTY(Transient)
	UPlayerCharacterStateWidget* StateWidget;

private:

	uint8 bAlive : 1;

	int32 PlayerId;
};

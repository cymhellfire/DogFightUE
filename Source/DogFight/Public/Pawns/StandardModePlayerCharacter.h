// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"
#include "Actors/ActorInterfaces.h"
#include "Interfaces/GameAnimatedCharacterInterface.h"
#include "GameFramework/Character.h"
#include "StandardModePlayerCharacter.generated.h"

UCLASS(Config=Game)
class DOGFIGHT_API AStandardModePlayerCharacter : public ACharacter, public IDamageableActorInterface, public IGameAnimatedCharacterInterface
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterDeadSignature);
	FCharacterDeadSignature OnCharacterDead;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterHealthChangedSignature, int32, NewHealth);
	FCharacterHealthChangedSignature OnCharacterHealthChanged;
	FCharacterHealthChangedSignature OnCharacterStrengthChanged;

	// Sets default values for this character's properties
	AStandardModePlayerCharacter();

#pragma region IDamageableActorInterface

	virtual UReceiveDamageComponent* GetDamageReceiveComponent() override;
	virtual void SetInvincible(bool bActive) override;
	virtual bool AddExtraArmor(FActorArmor& NewArmor) override;
	virtual bool RemoveExtraArmor(FActorArmor& TargetArmor) override;
#pragma endregion IDamageableActorInterface

#pragma region IGameAnimatedCharacterInterface
	virtual float PlayMontage(UAnimMontage* MontageToPlay) override;
#pragma endregion IGameAnimatedCharacterInterface

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnRep_UnitName();

	UFUNCTION()
	void OnRep_CurrentHealth();

	UFUNCTION()
	void OnRep_CurrentStrength();

	void Dead();

	/** Synchronize capsule component with ragdoll orientation. */
	void SynchronizeOrientationWithRagdoll(bool bIsFacingUp);

	void PreCacheRagdollPose();

	void DoCacheRagdollPose();

	void PostCacheRagdollPose();

	void RagdollAutoRecoverTick(); 

	UFUNCTION(BlueprintImplementableEvent, Category="Animation", meta=(DisplayName = "OnRagdollEnabled"))
	void K2_OnRagdollEnabled();

	UFUNCTION(BlueprintImplementableEvent, Category="Animation", meta=(DisplayName = "OnCacheRagdollPose"))
	void K2_OnCacheRagdollPose();

	UFUNCTION(BlueprintImplementableEvent, Category="Animation", meta=(DisplayName = "OnPostCacheRagdollPose"))
	void K2_OnPostCacheRagdollPose();

	UFUNCTION()
	void OnRep_SyncRagdollRotation();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayMontage(UAnimMontage* MontageToPlay);

public:
	/** Set name for this unit. */
	void SetUnitName(const FString& NewName);

	UFUNCTION(BlueprintCallable, Category="DogFight|Character")
	void SetCurrentHealth(int32 NewHealth);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|Character")
	void UnitNameChanged(const FText& PlayerName);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|Gameplay")
	void CurrentHealthChanged(const int32 NewHealth);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE class UDecalComponent* GetCursorToWorld() const { return CursorToWorld; }

	void SetCursorVisible(bool bVisible);

	/** Stop character movement immediately. */
	void StopMoveImmediately();

	/** Set the aiming direction and start rotating. */
	void SetAimingDirection(FVector NewDirection);

	int32 GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category="Animation", NetMulticast, Reliable)
	void MulticastSetRagdollActive(bool bActive);

	void SetRagdollActive(bool bActive);

	void RecoverStrength();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character", Config)
	int32 MaxBaseHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character", Config)
	int32 MaxStrength;

	/** The duration between ragdoll stop moving and character start to recover. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character", Config)
	float RagdollAutoRecoverDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character")
	float RagdollStopThreshold;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Unit", meta=(ExposeFunctionCategories = "ReceiveDamageComponent", AllowPrivateAccess = "true"))
	UReceiveDamageComponent* ReceiveDamageComponent;

	/** The target bone to follow during ragdoll activate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	FName FollowBoneName;

	/** The maximum height to detect ground when ragdoll activate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	float RagdollFloorDetectHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	FName PoseSlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	FName NeckBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	FName PelvisBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	UAnimMontage* GetUpFromFaceMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	UAnimMontage* GetUpFromBackMontage;

private:
	/** Current unit name. */
	UPROPERTY(Category="Unit", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), ReplicatedUsing=OnRep_UnitName)
	FString UnitName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess = "true"), ReplicatedUsing=OnRep_CurrentHealth)
	int32 CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess = "true"), ReplicatedUsing=OnRep_CurrentStrength)
	int32 CurrentStrength;

	/** A decal that projects to the cursor location. */
	UPROPERTY(Category=Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	UPROPERTY(Category=UI, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess="true"))
	class UWidgetComponent* WidgetComponent;

	bool bShowCursorToWorld;

	UPROPERTY(Category="Unit", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess="true"))
	bool bAlive;

	/** The desired rotation when character is aimed. */
	FRotator DesireFacingRotation;

	uint8 AimingState;

	/** The threshold angle when consider aiming is finished. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess="true"))
	float AimingApproximateAngle;

	/** Rotating speed while aiming. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess="true"))
	float AimingRotateSpeed;

	/** The initial local position of SkeletalMesh component. */
	FVector SkeletalMeshOffset;

	FTimerHandle RagdollRecoverTimerHandle;

	FTimerHandle RagdollAutoRecoverTimerHandle;

	float RagdollRecoverTimer;

	bool bRagdoll;

	bool bRagdollFacingUp;

	bool bRagdollAutoRecover;

	UPROPERTY(Replicated)
	FVector CacheBlastForce;

	UPROPERTY(ReplicatedUsing=OnRep_SyncRagdollRotation)
	FQuat SyncRagdollRotation;
};

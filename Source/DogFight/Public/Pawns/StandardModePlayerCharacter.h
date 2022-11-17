// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"
#include "Actors/Interfaces/DamageableActorInterface.h"
#include "Actors/Interfaces/BuffableActorInterface.h"
#include "Actors/Interfaces/GameplayTagsActorInterface.h"
#include "Actors/Interfaces/GameAnimatedCharacterInterface.h"
#include "Actors/Interfaces/WeaponCarrierInterface.h"
#include "GameFramework/Character.h"
#include "StandardModePlayerCharacter.generated.h"

class UCardDescObject;

UCLASS(Config=Game)
class DOGFIGHT_API AStandardModePlayerCharacter : public ACharacter, public IDamageableActorInterface, public IGameAnimatedCharacterInterface,
	public IBuffableActorInterface, public IGameplayTagsActorInterface, public IWeaponCarrierInterface
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterDelegateNoArgument);
	FCharacterDelegateNoArgument OnCharacterDead;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterHealthChangedSignature, int32, NewHealth);
	FCharacterHealthChangedSignature OnCharacterHealthChanged;
	FCharacterHealthChangedSignature OnCharacterStrengthChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCharacterRagdollStateChangedSignature, AStandardModePlayerCharacter*, Character, bool, bActive);
	FCharacterRagdollStateChangedSignature OnCharacterRagdollStateChanged;
	FCharacterRagdollStateChangedSignature OnCharacterReviveStateChanged;

	// Sets default values for this character's properties
	AStandardModePlayerCharacter();

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	void SetSupremeController(AController* NewController);

	AController* GetSupremeController() const { return SupremeController; }

#pragma region IDamageableActorInterface

	virtual UReceiveDamageComponent* GetDamageReceiveComponent() override;
	virtual void AddInvincibleFlags(int32 Flags) override;
	virtual void RemoveInvincibleFlags(int32 Flags) override;
	virtual bool AddExtraArmor(FActorArmor& NewArmor) override;
	virtual bool RemoveExtraArmor(FActorArmor& TargetArmor) override;
	virtual void SetHealthPercentage(float NewPercentage) override;
	virtual bool IsAlive() override { return bAlive; }
	virtual void ApplyDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
#pragma endregion IDamageableActorInterface

#pragma region IGameAnimatedCharacterInterface
	virtual float PlayMontage(UAnimMontage* MontageToPlay) override;
#pragma endregion IGameAnimatedCharacterInterface

#pragma region IBuffableActorInterface
	virtual UBuffQueue* GetBuffQueue() override;
#pragma endregion

#pragma region GameplayTagsActorInterface
	virtual void GetGameplayTags(FGameplayTagContainer& OutGameplayTags) override;
#pragma endregion

#pragma region WeaponCarrierInterface
	virtual UWeaponBase* GetCurrentWeapon() override { return CurrentWeapon; }
	virtual EWeaponType GetCurrentWeaponType() override;
	virtual struct FWeaponActionDisplayInfo GetNextActionDisplayInfoByInput(EWeaponActionInput Input) const override;
	virtual void EquipWeapon(UWeaponBase* NewWeapon) override;
	virtual void UnEquipWeapon() override;
	virtual void EnqueueInput(EWeaponActionInput NewInput) override;
	virtual void SetWeaponTargetActor(AActor* NewTarget) override;
	virtual void SetWeaponTargetLocation(FVector NewLocation) override;
	virtual void ClearWeaponTargetActor() override { WeaponTargetActor = nullptr; }
	virtual void MoveToActionDistance() override;
	virtual void SetActionDistance(float NewDistance) override;
	virtual AActor* GetWeaponTargetActor() override { return WeaponTargetActor; }
	virtual FWeaponCarrierWithOwnerSignature& GetWeaponEquippedEvent() override { return OnWeaponEquippedEvent; }
	virtual FWeaponCarrierWithOwnerSignature& GetWeaponActionFinishedEvent() override { return OnWeaponActionFinishedEvent; }
	virtual FWeaponCarrierWithOwnerSignature& GetCarrierReachActionDistanceEvent() override { return OnCarrierReachedActionDistanceEvent; }

	FWeaponCarrierWithOwnerSignature OnWeaponEquippedEvent;
	FWeaponCarrierWithOwnerSignature OnWeaponActionFinishedEvent;
	FWeaponCarrierWithOwnerSignature OnCarrierReachedActionDistanceEvent;
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnRep_UnitName();

	UFUNCTION()
	void OnRep_MaxBaseHealth();

	UFUNCTION()
	void OnRep_CurrentHealth();

	UFUNCTION()
	void OnRep_MaxStrength();

	UFUNCTION()
	void OnRep_CurrentStrength();

	void Dead();

	/** Synchronize capsule component with ragdoll orientation. */
	void SynchronizeOrientationWithRagdoll(bool bIsFacingUp);

	void PreCacheRagdollPose();

	void DoCacheRagdollPose();

	void PostCacheRagdollPose();

	void OnCharacterGetUp();

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

	UFUNCTION()
	void OnWeaponEquipped();

	UFUNCTION()
	void OnWeaponUnEquipped();

	UFUNCTION()
	void OnWeaponActionFinished();

	UFUNCTION()
	void OnPlayerRoundBegin(int32 PlayerId);

	UFUNCTION()
	void OnPlayerRoundEnd(int32 PlayerId);

	UFUNCTION()
	void OnTeleportFinished(class UActorTeleportComponent* Component);

public:
	/** Set name for this unit. */
	void SetUnitName(const FString& NewName);

	UFUNCTION(BlueprintCallable, Category="DogFight|Character")
	void SetCurrentHealth(float NewHealth);

	void SetMaxHealth(float NewMaxHealth);

	void SetCurrentStrength(float NewStrength);

	void SetMaxStrength(float NewMaxStrength);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|Character")
	void UnitNameChanged(const FText& PlayerName);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|Gameplay")
	void MaxHealthChanged(const int32 NewMaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|Gameplay")
	void CurrentHealthChanged(const int32 NewHealth);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|Gameplay")
	void MaxStrengthChanged(const int32 NewMaxStrength);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|Gameplay")
	void CurrentStrengthChanged(const int32 NewStrength);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void TakeStrengthCost(float StrengthCost, FVector KnockdownForce);

	FORCEINLINE class UDecalComponent* GetCursorToWorld() const { return CursorToWorld; }

	void SetCursorVisible(bool bVisible);

	/** Stop character movement immediately. */
	void StopMoveImmediately();

	/** Set the aiming direction and start rotating. */
	void SetAimingDirection(FVector NewDirection, TFunction<void()> Callback = nullptr);

	UFUNCTION(BlueprintCallable, Category="DogFight|Character")
	int32 GetCurrentHealth() const { return FMath::CeilToInt(CurrentHealth); }

	UFUNCTION(BlueprintCallable, Category="Animation", NetMulticast, Reliable)
	void MulticastSetRagdollActive(bool bActive);

	void SetRagdollActive(bool bActive);

	void AddForceToAllRagdollBodies(FVector Force);

	bool IsRagdollActive() const { return bRagdoll; }

	void RecoverStrength();

	void Revive();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAddFloatingText(const FText& NewText);

	/** Get BuffQueue of this character. */
	class UBuffQueue* GetBuffQueue() const;

	void CacheCurrentLocation();

	void ReturnToCachedLocation();

	void EquipTestWeapon();

	void AddCardDescObject(UCardDescObject* InObject);

protected:

	void ShowFloatingText(FText NewText);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character", Config, ReplicatedUsing=OnRep_MaxBaseHealth)
	int32 MaxBaseHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character", Config, ReplicatedUsing=OnRep_MaxStrength)
	int32 MaxStrength;

	/** The duration between ragdoll stop moving and character start to recover. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character", Config)
	float RagdollAutoRecoverDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character")
	float RagdollStopThreshold;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Unit", meta=(ExposeFunctionCategories = "ReceiveDamageComponent", AllowPrivateAccess = "true"))
	UReceiveDamageComponent* ReceiveDamageComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Unit", meta=(ExposeFunctionCategories = "TeleportComponent", AllowPrivateAccess = "true"))
	class UActorTeleportComponent* TeleportComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Unit", meta=(AllowPrivateAccess="true"))
	class UTestAttributeComponent* AttributeComponent;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Debug")
	TSubclassOf<UWeaponBase> TestWeaponClass;

	UPROPERTY(Transient, Replicated)
	TArray<UCardDescObject*> CardDescObjects;

private:
	/** Current unit name. */
	UPROPERTY(Category="Unit", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), ReplicatedUsing=OnRep_UnitName)
	FString UnitName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess = "true"), ReplicatedUsing=OnRep_CurrentHealth)
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess = "true"), ReplicatedUsing=OnRep_CurrentStrength)
	int32 CurrentStrength;

	/** A decal that projects to the cursor location. */
	UPROPERTY(Category=Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	UPROPERTY(Category=UI, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess="true"))
	class UWidgetComponent* CharacterStatusWidgetComponent;

	UPROPERTY(Category=UI, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPRivateAccess="true"))
	class UWidgetComponent* FloatingTextWidgetComponent;

	bool bShowCursorToWorld;

	UPROPERTY(Category="Unit", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess="true"))
	bool bAlive;

	/** The desired rotation when character is aimed. */
	FRotator DesireFacingRotation;

	TFunction<void()> FacingFinishCallback;

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

	class UCharacterFloatingTextPanelWidget* FloatingTextPanelWidget;

	AController* SupremeController;

	UPROPERTY()
	UWeaponBase* CurrentWeapon;

	UPROPERTY(Replicated)
	EWeaponType CurrentWeaponType;

	UPROPERTY()
	UWeaponBase* PendingWeapon;

	AActor* WeaponTargetActor;
	FVector WeaponTargetLocation;
	float WeaponActionDistance;
	bool bTracingActionDistance;
	FVector CachedLocation;
};

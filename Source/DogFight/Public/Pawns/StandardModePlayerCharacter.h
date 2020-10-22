// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"
#include "ActorInterfaces.h"
#include "GameFramework/Character.h"
#include "StandardModePlayerCharacter.generated.h"

UCLASS(Config=Game)
class DOGFIGHT_API AStandardModePlayerCharacter : public ACharacter, public IDamageableActorInterface
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterDeadSignature);
	FCharacterDeadSignature OnCharacterDead;

	// Sets default values for this character's properties
	AStandardModePlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character", Config)
	int32 MaxBaseHealth;

#pragma region IDamageableActorInterface

	virtual UReceiveDamageComponent* GetDamageReceiveComponent() override;
	virtual void SetPhysicalArmor(int32 NewArmor) override;
	virtual int32 GetPhysicalArmor() const override;
	virtual void SetMagicalArmor(int32 NewArmor) override;
	virtual int32 GetMagicalArmor() const override;
#pragma endregion IDamageableActorInterface

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnRep_UnitName();

	UFUNCTION()
	void OnRep_CurrentHealth();

	void Dead();

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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Unit", meta=(ExposeFunctionCategories = "ReceiveDamageComponent", AllowPrivateAccess = "true"))
	UReceiveDamageComponent* ReceiveDamageComponent;

private:
	/** Current unit name. */
	UPROPERTY(Category="Unit", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), ReplicatedUsing=OnRep_UnitName)
	FString UnitName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess = "true"), ReplicatedUsing=OnRep_CurrentHealth)
	int32 CurrentHealth;

	/** A decal that projects to the cursor location. */
	UPROPERTY(Category=Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	UPROPERTY(Category=UI, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess="true"))
	class UWidgetComponent* WidgetComponent;

	bool bShowCursorToWorld;

	/** The desired rotation when character is aimed. */
	FRotator DesireFacingRotation;

	uint8 AimingState;

	/** The threshold angle when consider aiming is finished. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess="true"))
	float AimingApproximateAngle;

	/** Rotating speed while aiming. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess="true"))
	float AimingRotateSpeed;
};

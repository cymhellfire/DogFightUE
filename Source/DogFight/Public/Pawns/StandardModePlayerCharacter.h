// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"
#include "GameFramework/Character.h"
#include "StandardModePlayerCharacter.generated.h"

UCLASS(Config=Game)
class DOGFIGHT_API AStandardModePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AStandardModePlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character", Config)
	int32 MaxBaseHealth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnRep_UnitName();

	UFUNCTION()
	void OnRep_CurrentHealth();

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
};

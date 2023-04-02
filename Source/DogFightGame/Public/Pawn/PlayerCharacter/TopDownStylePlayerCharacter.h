#pragma once

#include "DamageReceiver/DamageReceiverComponent.h"
#include "GameFramework/Character.h"
#include "GameObject/Component/WidgetLocatorComponent.h"
#include "Interface/DamageReceiverActorInterface.h"
#include "TopDownStylePlayerCharacter.generated.h"

class ATopDownStylePlayerCharacter;
class UPlayerCharacterStateWidget;
class URagdollComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTopDownStylePlayerCharacterDeadEvent, ATopDownStylePlayerCharacter*, Character);

UCLASS()
class DOGFIGHTGAME_API ATopDownStylePlayerCharacter : public ACharacter, public IDamageReceiverActorInterface
{
	GENERATED_BODY()
public:
	ATopDownStylePlayerCharacter();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	FVector GetProjectileSpawnLocation() const;

	UFUNCTION(BlueprintCallable)
	void SetRagdollEnabled(bool bEnable);

	// DamageReceiverActorInterface
	virtual UDamageReceiverComponent* GetDamageReceiverComponent() override
	{
		return DamageReceiverComponent;
	}

protected:
	void InitializeStateWidget();
	void DeinitializeStateWidget();

	virtual void Dead();

	UFUNCTION()
	void OnHealthChanged(float CurHealth, float MaxHealth);

	UFUNCTION()
	void OnNoHealth();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector ProjectileSpawnOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UPlayerCharacterStateWidget> StateWidgetClass;

	UPROPERTY(BlueprintAssignable, Category="TopDownStylePlayerCharacter")
	FTopDownStylePlayerCharacterDeadEvent OnCharacterDead;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDamageReceiverComponent* DamageReceiverComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetLocatorComponent* StateWidgetLocatorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URagdollComponent* RagdollComponent;

	UPROPERTY(Transient)
	UPlayerCharacterStateWidget* StateWidget;

private:

	uint8 bAlive : 1;
};

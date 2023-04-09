#pragma once

#include "AITypes.h"
#include "DamageReceiver/DamageReceiverComponent.h"
#include "GameFramework/Character.h"
#include "GameObject/Component/WidgetLocatorComponent.h"
#include "Interface/DamageReceiverActorInterface.h"
#include "TopDownStylePlayerCharacter.generated.h"

class ATopDownStylePlayerCharacter;
class UPlayerCharacterStateWidget;
class URagdollComponent;
class UPathFollowingComponent;
struct FPathFollowingResult;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTopDownStylePlayerCharacterDeadEvent, ATopDownStylePlayerCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTopDownStylePlayerCharacterMoveFinishedEvent);

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

	void OnMoveFinished(FAIRequestID RequestID, const FPathFollowingResult& Result);

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

	UPROPERTY(BlueprintAssignable, Category="TopDownStylePlayerCharacter")
	FTopDownStylePlayerCharacterMoveFinishedEvent OnCharacterMoveFinished;

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

	TWeakObjectPtr<UPathFollowingComponent> PathFollowingComponent;
};

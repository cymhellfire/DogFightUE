#pragma once

#include "DamageReceiver/DamageReceiverComponent.h"
#include "GameFramework/Character.h"
#include "GameObject/Component/WidgetLocatorComponent.h"
#include "Interface/DamageReceiverActorInterface.h"
#include "FreeForAllPlayerCharacter.generated.h"

class UPlayerCharacterStateWidget;

UCLASS()
class DOGFIGHTGAME_API AFreeForAllPlayerCharacter : public ACharacter, public IDamageReceiverActorInterface
{
	GENERATED_BODY()
public:
	AFreeForAllPlayerCharacter();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	FVector GetProjectileSpawnLocation() const;

	// DamageReceiverActorInterface
	virtual UDamageReceiverComponent* GetDamageReceiverComponent() override
	{
		return DamageReceiverComponent;
	}

protected:
	void InitializeStateWidget();
	void DeinitializeStateWidget();

	UFUNCTION()
	void OnHealthChanged(float CurHealth, float MaxHealth);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector ProjectileSpawnOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UPlayerCharacterStateWidget> StateWidgetClass;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDamageReceiverComponent* DamageReceiverComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetLocatorComponent* StateWidgetLocatorComponent;

	UPROPERTY(Transient)
	UPlayerCharacterStateWidget* StateWidget;
};

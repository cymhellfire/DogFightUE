// Dog Fight Game Code By CYM.

#pragma once

#include "DogFight.h"
#include "GameFramework/Character.h"
#include "StandardModePlayerCharacter.generated.h"

UCLASS()
class DOGFIGHT_API AStandardModePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AStandardModePlayerCharacter();

	/** Set name for this unit. */
	void SetUnitName(const FString& NewName);

	UFUNCTION(BlueprintImplementableEvent, Category="DogFight|Character")
	void UnitNameChanged(const FText& PlayerName);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnRep_UnitName();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE class UDecalComponent* GetCursorToWorld() const { return CursorToWorld; }
	
private:
	/** Current unit name. */
	UPROPERTY(Category="Unit", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), ReplicatedUsing=OnRep_UnitName)
	FString UnitName;

	/** A decal that projects to the cursor location. */
	UPROPERTY(Category=Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	UPROPERTY(Category=UI, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess="true"))
	class UWidgetComponent* WidgetComponent;
};

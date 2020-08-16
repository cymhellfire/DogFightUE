// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StandardModePlayerCharacter.generated.h"

UCLASS()
class DOGFIGHT_API AStandardModePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AStandardModePlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE class UDecalComponent* GetCursorToWorld() const { return CursorToWorld; }

private:
	/** A decal that projects to the cursor location. */
	UPROPERTY(Category=Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;
};

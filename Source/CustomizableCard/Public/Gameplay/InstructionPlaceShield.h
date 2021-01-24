// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "InstructionAddBuff.h"
#include "Interfaces/GameShieldInterface.h"
#include "InstructionPlaceShield.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMIZABLECARD_API UInstructionPlaceShield : public UInstructionAddBuff
{
	GENERATED_BODY()
public:
	UInstructionPlaceShield(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool HandleActorTarget(AActor* Target) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Instruction", meta=(MustImplement="GameShieldInterface"))
	TSubclassOf<AActor> ShieldClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Instruction")
	EShieldBlockType ShieldBlockType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Instruction")
	int32 ShieldLifetime;
};

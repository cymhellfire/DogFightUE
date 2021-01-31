// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/HandleTargetInstructionBase.h"
#include "InstructionAddBuff.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMIZABLECARD_API UInstructionAddBuff : public UHandleTargetInstructionBase
{
	GENERATED_BODY()

public:
	UInstructionAddBuff(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool HandleActorTarget(AActor* Target) override;

	/** Triggered every time a buff created. Use this to initialize new buff instance. */
	virtual void OnBuffCreated(class IGameBuffInterface* BuffActor) {}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Instruction", meta=(MustImplement="GameBuffInterface"))
	TSubclassOf<AActor> BuffClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Instruction")
	int32 BuffLifetime;
};

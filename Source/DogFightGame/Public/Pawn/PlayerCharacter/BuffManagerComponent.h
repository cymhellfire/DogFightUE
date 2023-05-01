// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffManagerComponent.generated.h"

class UNewBuffBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOGFIGHTGAME_API UBuffManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBuffManagerComponent();

	UFUNCTION(BlueprintCallable, Category="BuffManagerComponent")
	void AddBuff(UNewBuffBase* InBuff);

	UFUNCTION(BlueprintCallable, Category="BuffManagerComponent")
	void RemoveBuff(UNewBuffBase* InBuff);

protected:
	UPROPERTY(Transient)
	TArray<UNewBuffBase*> AppliedBuffs;
};

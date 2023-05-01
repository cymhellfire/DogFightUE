// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameObject/Buff/BuffCommonDef.h"
#include "BuffManagerComponent.generated.h"

class UNewBuffBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBuffManagerComponentDoCheckFinishEvent);

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

	UFUNCTION(BlueprintCallable, Category="BuffManagerComponent")
	void DoBuffCheck(TEnumAsByte<EBuffCheckType::Type> InCheckType);

protected:
	void StartDoBuffCheck();

	void OnBuffCheckFinished();

	void OnAllBuffCheckFinished();

public:
	UPROPERTY(BlueprintAssignable, Category="BuffManagerComponent")
	FBuffManagerComponentDoCheckFinishEvent OnDoCheckFinished;

protected:
	UPROPERTY(Transient)
	TArray<UNewBuffBase*> AppliedBuffs;

	int32 DoCheckIndex;
	EBuffCheckType::Type CurCheckType;
	FDelegateHandle DoCheckHandle;
};

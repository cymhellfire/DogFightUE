// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShieldManager.generated.h"

class AShieldBase;

UCLASS()
class DOGFIGHT_API AShieldManager : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AShieldManager();

	void RegisterShield(AShieldBase* NewShield);

	void UnregisterShield(AShieldBase* Shield);

	TArray<AShieldBase*> GetAllShield() const { return ShieldList; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShieldManagerDelegateWithInstancePointer, AShieldBase*, NewShield);
	FShieldManagerDelegateWithInstancePointer OnShieldRegistered;
	FShieldManagerDelegateWithInstancePointer OnShieldUnregistered;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ShieldManager")
	TArray<AShieldBase*> ShieldList;

};

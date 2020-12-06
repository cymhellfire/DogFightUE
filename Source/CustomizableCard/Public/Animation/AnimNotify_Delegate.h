// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Delegate.generated.h"

UENUM(BlueprintType)
enum class EAnimNotifyDelegateType : uint8
{
	DT_Default,
	DT_LaunchProjectile,
	DT_SpawnActor,
	DT_ActionFinished,
};

/**
 * 
 */
UCLASS()
class CUSTOMIZABLECARD_API UAnimNotify_Delegate : public UAnimNotify
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnimNotifyDelegateSignature, UAnimNotify_Delegate*, NotifyInstance);
	FAnimNotifyDelegateSignature OnAnimNotifyTriggered;

	UAnimNotify_Delegate();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotifyDelegate")
	EAnimNotifyDelegateType DelegateType;
};

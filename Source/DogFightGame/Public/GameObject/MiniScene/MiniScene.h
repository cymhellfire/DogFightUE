// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiniScene.generated.h"

class UChildActorRegistryComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class DOGFIGHTGAME_API AMiniScene : public AActor
{
	GENERATED_BODY()

public:
	AMiniScene();

	UFUNCTION(BlueprintCallable, Category="MiniScene")
	void AddChildActor(AActor* InActor, FName ComponentName);

	UFUNCTION(BlueprintCallable, Category="MiniScene")
	void RemoveChildActor(AActor* InActor);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Let this mini scene become the view target.
	 */
	void HijackViewTarget();

	/**
	 * Release the view target from here. 
	 */
	void ReleaseViewTarget();

private:
	UPROPERTY(EditAnywhere, Category="MiniScene")
	USceneComponent* CameraRoot;

	UPROPERTY(EditAnywhere, Category="MiniScene")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, Category="MiniScene")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Category="MiniScene")
	UChildActorRegistryComponent* ChildActorRegistry;

	uint8 bHijackViewTarget: 1;

	TWeakObjectPtr<AActor> OrgViewTarget;
};

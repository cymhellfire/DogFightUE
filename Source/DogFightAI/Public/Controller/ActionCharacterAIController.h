// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ActionCharacterAIController.generated.h"

DECLARE_MULTICAST_DELEGATE(FDogFightAIControllerReachStopDistance);

UENUM(BlueprintType)
enum class EDogFightAIControllerState : uint8
{
	DFACS_None,
	DFACS_Moving,
};

UCLASS()
class DOGFIGHTAI_API AActionCharacterAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActionCharacterAIController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveToTargetWithStopDistance(const FVector& Target, float InStopDistance);

protected:
	void SetState(EDogFightAIControllerState NewState);
	void TrackStopDistance();

	void OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result);

public:
	FDogFightAIControllerReachStopDistance OnReachStopDistance;

protected:
	EDogFightAIControllerState ControllerState;

	bool bManualStop ;		// Stop path following by set state
	float SqrStopDistance;
	FVector MoveTarget;
};

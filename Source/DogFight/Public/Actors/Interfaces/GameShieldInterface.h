// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"
#include "GameShieldInterface.generated.h"

UENUM(BlueprintType)
enum class EShieldBlockType : uint8
{
	SBT_Enemy		UMETA(DisplayName="Enemy"),
	SBT_Ally		UMETA(DisplayName="Ally"),
	SBT_NotMine		UMETA(DisplayName="NotMine"),
	SBT_All			UMETA(DisplayName="All"),
};

UINTERFACE(MinimalAPI, Blueprintable)
class UGameShieldInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface that allows shields interact with card instruction.
 */
class IGameShieldInterface
{
	GENERATED_BODY()

public:

	/** Set owner player controller. */
	virtual void SetSourcePlayerController(AController* NewController) {};

	/** Set the shield block type to this shield. */
	virtual void SetShieldBlockType(EShieldBlockType NewBlockType) {};

	/** Set the target actor to attach. */
	virtual void SetAttachActor(AActor* NewParent) {};

	/** Set life time of shield. */
	virtual void SetLifetime(int32 Lifetime) {};
};
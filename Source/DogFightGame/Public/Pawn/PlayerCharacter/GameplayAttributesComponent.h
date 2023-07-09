// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "UnrealIntegration/UObject/AttributeBasedComponent.h"
#include "GameplayAttributesComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOGFIGHTGAME_API UGameplayAttributesComponent : public UAttributeBasedComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGameplayAttributesComponent(const FObjectInitializer& ObjectInitializer);

	/**
	 * Initialize component with specified attribute configuration.
	 * @param ConfigId Id of config to initialize with.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="GameplayAttributesComponent")
	void InitAttributeConfig(int32 ConfigId);
};

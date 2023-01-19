#pragma once

#include "Common/InputMappingDef.h"
#include "GameService/LuaGameService.h"
#include "GameInputService.generated.h"

class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;

UCLASS()
class DOGFIGHTGAME_API UGameInputService : public ULuaGameService
{
	GENERATED_BODY()

public:
	/**
	 * Broadcast add input mapping across the network.
	 * @param MappingType			Input mapping type.
	 */
	UFUNCTION(BlueprintCallable, Category="GameInput")
	void MulticastAddInputMapping(EInputMappingType::Type MappingType);

	/**
	 * Broadcast remove input mapping across the network.
	 * @param MappingType			Input mapping type.
	 */
	UFUNCTION(BlueprintCallable, Category="GameInput")
	void MulticastRemoveInputMapping(EInputMappingType::Type MappingType);

	void AddInputMapping(EInputMappingType::Type MappingType);

	void RemoveInputMapping(EInputMappingType::Type MappingType);

	virtual FString GetModuleName_Implementation() const override
	{
		return "DogFight.Services.GameInputService";
	}

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="GameInput")
	FString BP_GetInputMappingPath(EInputMappingType::Type MappingType);

	UInputMappingContext* GetInputMappingContextByType(EInputMappingType::Type InType);

	UEnhancedInputLocalPlayerSubsystem* GetInputSubsystemFromLocalPlayer() const;
};

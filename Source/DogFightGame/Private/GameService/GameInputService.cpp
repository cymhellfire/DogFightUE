#include "GameService/GameInputService.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Common/DogFightGameLog.h"
#include "FunctionLibrary/CommonGameFlowFunctionLibrary.h"
#include "FunctionLibrary/CommonGameplayFunctionLibrary.h"
#include "GameMode/TopDownStyleGameMode.h"
#include "PlayerController/TopDownStylePlayerController.h"

void UGameInputService::MulticastAddInputMapping(EInputMappingType::Type MappingType)
{
	if (auto GameMode = UCommonGameFlowFunctionLibrary::GetCurrentTopDownStyleGameMode(this))
	{
		auto PCList = GameMode->GetAllPlayerControllers();
		for (auto PC : PCList)
		{
			if (PC.IsValid())
			{
				PC->ClientAddInputMapping(MappingType);
			}
		}
	}
}

void UGameInputService::MulticastRemoveInputMapping(EInputMappingType::Type MappingType)
{
	if (auto GameMode = UCommonGameFlowFunctionLibrary::GetCurrentTopDownStyleGameMode(this))
	{
		auto PCList = GameMode->GetAllPlayerControllers();
		for (auto PC : PCList)
		{
			if (PC.IsValid())
			{
				PC->ClientRemoveInputMapping(MappingType);
			}
		}
	}
}

void UGameInputService::AddInputMappingByPlayerId(int32 InPlayerId, EInputMappingType::Type MappingType)
{
	if (auto PC = UCommonGameplayFunctionLibrary::GetPlayerControllerById(this, InPlayerId))
	{
		PC->ClientAddInputMapping(MappingType);
	}
}

void UGameInputService::RemoveInputMappingByPlayerId(int32 InPlayerId, EInputMappingType::Type MappingType)
{
	if (auto PC = UCommonGameplayFunctionLibrary::GetPlayerControllerById(this, InPlayerId))
	{
		PC->ClientRemoveInputMapping(MappingType);
	}
}

void UGameInputService::AddInputMapping(EInputMappingType::Type MappingType)
{
	auto MappingContext = GetInputMappingContextByType(MappingType);
	if (!MappingContext)
	{
		return;
	}

	// Add new IMC to local player
	if (auto InputSystem = GetInputSubsystemFromLocalPlayer())
	{
		InputSystem->AddMappingContext(MappingContext, 0);
	}
}

void UGameInputService::RemoveInputMapping(EInputMappingType::Type MappingType)
{
	auto MappingContext = GetInputMappingContextByType(MappingType);
	if (!MappingContext)
	{
		return;
	}

	// Remove IMC from local player
	if (auto InputSystem = GetInputSubsystemFromLocalPlayer())
	{
		InputSystem->RemoveMappingContext(MappingContext);
	}
}

UInputMappingContext* UGameInputService::GetInputMappingContextByType(EInputMappingType::Type InType)
{
	FString ResPath = BP_GetInputMappingPath(InType);
	if (ResPath.IsEmpty())
	{
		return nullptr;
	}

	// Load resource
	UObject* ResObject = nullptr;
	FSoftObjectPtr SoftPath(ResPath);
	if(!SoftPath.IsValid())
	{
		ResObject = SoftPath.LoadSynchronous();
	}
	else
	{
		ResObject = SoftPath.Get();
	}

	UInputMappingContext* MappingContext = Cast<UInputMappingContext>(ResObject);
	if (!MappingContext)
	{
		UE_LOG(LogDogFightGame, Error, TEXT("[UGameInputService] Invalid Input Mapping Context was loaded."));
		return nullptr;
	}

	return MappingContext;
}

UEnhancedInputLocalPlayerSubsystem* UGameInputService::GetInputSubsystemFromLocalPlayer() const
{
	if (auto LocalPlayer = UCommonGameFlowFunctionLibrary::GetLocalPlayer(this))
	{
		return LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	}

	return nullptr;
}

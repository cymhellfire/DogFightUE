#pragma once

#include "CoreMinimal.h"
#include "CommonFunctionLibraryBase.generated.h"

class ATopDownStyleGameMode;
class ATopDownStyleGameState;

UCLASS()
class DOGFIGHTGAME_API UCommonFunctionLibraryBase : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static ATopDownStyleGameMode* GetCurrentTopDownStyleGameMode();

	static ATopDownStyleGameState* GetCurrentTopDownStyleGameState();
};

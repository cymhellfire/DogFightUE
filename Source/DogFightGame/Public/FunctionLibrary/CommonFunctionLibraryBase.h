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
	static ATopDownStyleGameMode* GetCurrentTopDownStyleGameMode(const UObject* WorldContextObject);

	static ATopDownStyleGameState* GetCurrentTopDownStyleGameState(const UObject* WorldContextObject);
};

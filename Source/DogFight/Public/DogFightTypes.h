#pragma once

#include "DogFightTypes.generated.h"

UENUM()
namespace EReturnToMainMenuReason
{
	enum Type
	{
		Unknown			UMETA(DisplayName="NetError_Unknown"),
		LostConnect		UMETA(DisplayName="NetError_LostConnect"),
		HostLeft		UMETA(DisplayName="NetError_HostLeft"),
	};
}
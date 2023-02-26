#include "GameService/DamageService.h"

#include "Common/DogFightGameLog.h"

void UDamageService::RegisterDamageType(int32 Id, UExtendedDamageInstance* Instance)
{
	if (DamageTypeMap.Contains(Id))
	{
		UE_LOG(LogDogFightGame, Error, TEXT("[DamageService] Duplicated damage config id detected: %d"), Id);
		return;
	}

	DamageTypeMap.Add(Id, Instance);
}

UExtendedDamageInstance* UDamageService::GetDamageInstanceById(int32 InId) const
{
	if (DamageTypeMap.Contains(InId))
	{
		return DamageTypeMap[InId];
	}
	return nullptr;
}

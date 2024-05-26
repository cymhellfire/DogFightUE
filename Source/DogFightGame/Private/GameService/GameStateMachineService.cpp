// Dog Fight Game Code By CYM.


#include "GameService/GameStateMachineService.h"

#include "Common/DogFightGameLog.h"

void UGameStateMachineService::Startup()
{
	Super::Startup();

	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &UGameStateMachineService::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UGameStateMachineService::OnPostLoadMap);
}

void UGameStateMachineService::Shutdown()
{
	Super::Shutdown();

	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

void UGameStateMachineService::OnPreLoadMap(const FWorldContext& WorldContext, const FString& MapName)
{
	DFLog(TEXT("New map: %s"), *MapName);
}

void UGameStateMachineService::OnPostLoadMap(UWorld* InWorld)
{
#if WITH_EDITOR
	// Remove map prefix in PIE
	int32 StartIndex = -1;
	auto MapName = InWorld->GetMapName();
	MapName.FindLastChar('_', StartIndex);
	MapName.MidInline(StartIndex + 1, MapName.Len() - StartIndex - 1);
#else
	const auto MapName = InWorld->GetMapName();
#endif
	DFLog(TEXT("New map: %s"), *MapName);

	OnPostLoadMapScript(MapName);
}

FString UGameStateMachineService::GetShortMapName(const FString& InMapPath) const
{
	int32 StartIndex = -1;
	InMapPath.FindLastChar('.', StartIndex);
	if (StartIndex != -1)
	{
		return InMapPath.Mid(StartIndex + 1, InMapPath.Len() - StartIndex - 1);
	}
	return InMapPath;
}

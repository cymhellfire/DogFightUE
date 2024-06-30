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

void UGameStateMachineService::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OnTickScript(DeltaTime);
}

void UGameStateMachineService::OnPreLoadMap(const FWorldContext& WorldContext, const FString& MapName)
{
	DFLog(LogDogFightGame, TEXT("New map: %s"), *MapName);
}

void UGameStateMachineService::OnPostLoadMap(UWorld* InWorld)
{
	auto MapName = InWorld->RemovePIEPrefix(InWorld->GetOutermost()->GetName());
	DFLog(LogDogFightGame, TEXT("New map: %s"), *MapName);

	OnPostLoadMapScript(MapName);
}

// Dog Fight Game Code By CYM.


#include "GameObject/MiniScene/MiniScene.h"

#include "Camera/CameraComponent.h"
#include "FunctionLibrary/LuaIntegrationFunctionLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameObject/MiniScene/ChildActorRegistryComponent.h"

#define FORWARD_FUNC_CALL(MemberName, FuncName, ...)		\
	if (IsValid(MemberName))	\
	{	\
		MemberName->FuncName(__VA_ARGS__);	\
	}

AMiniScene::AMiniScene()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Create camera component
	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	CameraRoot->SetupAttachment(RootComponent);
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(CameraRoot);
	SpringArmComponent->TargetArmLength = 400;
	SpringArmComponent->bDoCollisionTest = false;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Child actor registry
	ChildActorRegistry = CreateDefaultSubobject<UChildActorRegistryComponent>(TEXT("ChildActorRegistry"));

	// Initial values
	bHijackViewTarget = false;
}

void AMiniScene::BeginPlay()
{
	Super::BeginPlay();

	HijackViewTarget();
}

void AMiniScene::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ReleaseViewTarget();
}

void AMiniScene::HijackViewTarget()
{
	if (bHijackViewTarget)
	{
		return;
	}

	if (auto PC = ULuaIntegrationFunctionLibrary::GetFirstLocalPlayerController(this))
	{
		OrgViewTarget = PC->GetViewTarget();
		PC->SetViewTarget(this);
	}
}

void AMiniScene::ReleaseViewTarget()
{
	if (!bHijackViewTarget)
	{
		return;
	}

	if (auto PC = ULuaIntegrationFunctionLibrary::GetFirstLocalPlayerController(this))
	{
		if (OrgViewTarget.IsValid())
		{
			PC->SetViewTarget(OrgViewTarget.Get());
		}
		else
		{
			PC->SetViewTarget(PC->GetPawnOrSpectator());
		}
	}
}

void AMiniScene::AddChildActor(AActor* InActor, FName ComponentName)
{
	FORWARD_FUNC_CALL(ChildActorRegistry, AddChildActor, InActor, ComponentName)
}

void AMiniScene::RemoveChildActor(AActor* InActor)
{
	FORWARD_FUNC_CALL(ChildActorRegistry, RemoveChildActor, InActor)
}

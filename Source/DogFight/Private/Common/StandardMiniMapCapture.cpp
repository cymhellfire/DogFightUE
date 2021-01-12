// Dog Fight Game Code By CYM.


#include "Common/StandardMiniMapCapture.h"

#include "Game/StandardGameState.h"
#include "DogFight/DogFight.h"

AStandardMiniMapCapture::AStandardMiniMapCapture(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetCaptureComponent2D()->bCaptureEveryFrame = false;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	MiniMapHeight = 256;
	MiniMapWidth = 256;
	RootComponent->SetWorldRotation(FRotator(-90, 0, 0));
}

void AStandardMiniMapCapture::BeginPlay()
{
	Super::BeginPlay();

	if (!GetCaptureComponent2D()->TextureTarget || MiniMapWidth != GetCaptureComponent2D()->TextureTarget->GetSurfaceWidth()
		|| MiniMapHeight != GetCaptureComponent2D()->TextureTarget->GetSurfaceHeight())
	{
		MiniMapView = NewObject<UTextureRenderTarget2D>();
		MiniMapView->InitAutoFormat(MiniMapWidth, MiniMapHeight);
		GetCaptureComponent2D()->TextureTarget = MiniMapView;
		bTextureChanged = true;
	}

	RootComponent->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));
	AStandardGameState* MyGameState = GetWorld()->GetGameState<AStandardGameState>();
	if (MyGameState != nullptr)
	{
		MyGameState->MiniMapCamera = this;
	}
	CachedFOV = GetCaptureComponent2D()->FOVAngle;
	CachedLocation = RootComponent->GetComponentLocation();
	CachedOrthoWidth = GetCaptureComponent2D()->OrthoWidth;
	UpdateWorldBounds();
}

void AStandardMiniMapCapture::Tick(float DeltaSeconds)
{
	if (CachedFOV != GetCaptureComponent2D()->FOVAngle || CachedLocation != RootComponent->GetComponentLocation() ||
		CachedOrthoWidth != GetCaptureComponent2D()->OrthoWidth || bTextureChanged)
	{
		bTextureChanged = false;
		CachedFOV = GetCaptureComponent2D()->FOVAngle;
		CachedLocation = RootComponent->GetComponentLocation();
		CachedOrthoWidth = GetCaptureComponent2D()->OrthoWidth;
		UpdateWorldBounds();
	}
}

void AStandardMiniMapCapture::UpdateWorldBounds()
{
	AStandardGameState* MyGameState = GetWorld()->GetGameState<AStandardGameState>();
	if (MyGameState != nullptr)
	{
		TArray<FVector> Points;
		FVector const CamLocation = RootComponent->GetComponentLocation();

		if (GetCaptureComponent2D()->ProjectionType == ECameraProjectionMode::Perspective)
		{
			const float DistanceFromGround = CamLocation.Z - GroundLevel;
			const float Alpha = FMath::DegreesToRadians(GetCaptureComponent2D()->FOVAngle / 2);
			const float MaxVisibleDistance = (DistanceFromGround / FMath::Cos(Alpha)) * FMath::Sin(Alpha);

			Points.Add(FVector(CamLocation.X + MaxVisibleDistance, CamLocation.Y + MaxVisibleDistance, GroundLevel));
			Points.Add(FVector(CamLocation.X - MaxVisibleDistance, CamLocation.Y - MaxVisibleDistance, GroundLevel));
		}
		else
		{
			const float ViewExtent = GetCaptureComponent2D()->OrthoWidth / 2.f;
			
			Points.Add(FVector(CamLocation.X + ViewExtent, CamLocation.Y + ViewExtent, GroundLevel));
			Points.Add(FVector(CamLocation.X - ViewExtent, CamLocation.Y - ViewExtent, GroundLevel));
		}

		MyGameState->WorldBounds = FBox(Points);
		GetCaptureComponent2D()->UpdateContent();
		UE_LOG(LogDogFight, Display, TEXT("World bounds updated. %d"), GetCaptureComponent2D()->ProjectionType.GetValue());
	}
}

#if WITH_EDITOR
void AStandardMiniMapCapture::EditorApplyRotation(const FRotator& DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	const FRotator FilteredRotation(0, DeltaRotation.Yaw, 0);
	Super::EditorApplyRotation(FilteredRotation, bAltDown, bShiftDown, bCtrlDown);
}

void AStandardMiniMapCapture::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != nullptr ? PropertyThatChanged->GetFName() : NAME_None;

	if (PropertyName == FName(TEXT("RelativeRotation")))
	{
		const FRotator ChangedRotation = RootComponent->GetComponentRotation();
		RootComponent->SetWorldRotation(FRotator(-90, 0, ChangedRotation.Roll));
	}
}
#endif

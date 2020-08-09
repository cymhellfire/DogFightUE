// Dog Fight Game Code By CYM.


#include "StandardHUD.h"

#include "StandardGameState.h"
#include "StandardModePlayerController.h"
#include "MathHelper.h"
#include "DogFight/DogFight.h"

AStandardHUD::AStandardHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MiniMapMargin = 40;
	MiniMapPoints.Init(FVector2D::ZeroVector, 4);
}

void AStandardHUD::DrawHUD()
{
	if (GEngine && GEngine->GameViewport)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		UIScale = ViewportSize.X / 2048.f;
	}

	Super::DrawHUD();

	AStandardGameState const* const MyGameState = GetWorld()->GetGameState<AStandardGameState>();
	if (MyGameState != nullptr)
	{
		DrawMiniMap();
	}
}

void AStandardHUD::DrawMiniMap()
{
	AStandardModePlayerController* const PC = Cast<AStandardModePlayerController>(PlayerOwner);
	AStandardGameState const* const MyGameState = GetWorld()->GetGameState<AStandardGameState>();

	if (PC && MyGameState && MyGameState->MiniMapCamera.IsValid())
	{
		UTexture* MiniMapTexture = Cast<UTexture>(MyGameState->MiniMapCamera->GetCaptureComponent2D()->TextureTarget);
		const float MapWidth = (MyGameState->MiniMapCamera->MiniMapWidth - MiniMapMargin) * UIScale;
		const float MapHeight = (MyGameState->MiniMapCamera->MiniMapHeight - MiniMapMargin) * UIScale;
		const float ScaledMargin = MiniMapMargin * UIScale;
		const FVector WorldCenter = MyGameState->WorldBounds.GetCenter();
		const FVector WorldExtent = MyGameState->WorldBounds.GetExtent();
		const FRotationMatrix RotationMatrix(FRotator(0.f, 270.f, 0.f));

		const FVector2D Offset(ScaledMargin + (MapWidth / 2.f), Canvas->ClipY - (MapHeight / 2.f) - ScaledMargin);

		// Draw map texture
		if (MiniMapTexture)
		{
			FCanvasTileItem MapTileItem(FVector2D(0.f, 0.f), FVector2D(0.f, 0.f), FLinearColor::White);
			MapTileItem.Texture = MiniMapTexture->Resource;
			MapTileItem.Size = FVector2D(MapWidth, MapHeight);
			MapTileItem.BlendMode = SE_BLEND_Opaque;
			Canvas->DrawItem(MapTileItem, FVector2D(ScaledMargin, Canvas->ClipY - MapHeight - ScaledMargin));
		}

		// Update camera frustum points
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->Player);
		FVector2D ScreenCorners[4] = {
			FVector2D(0, 0),
			FVector2D(Canvas->ClipX, 0),
			FVector2D(Canvas->ClipX, Canvas->ClipY),
			FVector2D(0, Canvas->ClipY)
		};
		const FPlane GroundPlane = FPlane(FVector(0, 0, MyGameState->WorldBounds.Max.Z), FVector::UpVector);
		for (int i = 0; i < 4; i++)
		{
			FVector RayOrigin, RayDirection;
			FMathHelper::DeprojectScreenToWorld(ScreenCorners[i], LocalPlayer, RayOrigin, RayDirection);
			const FVector GroundPoint = FMathHelper::IntersectRayWithPlane(RayOrigin, RayDirection, GroundPlane);
			const FVector CenterRelativeLocation = RotationMatrix.TransformPosition(GroundPoint - WorldCenter);
			MiniMapPoints[i] = FVector2D(CenterRelativeLocation.X / WorldExtent.X, CenterRelativeLocation.Y / WorldExtent.Y);

			if (DrawDebugLines)
			{
				FCanvasLineItem DebugFrustumLine = FCanvasLineItem(
					(i == 0 ? MiniMapPoints[3] : MiniMapPoints[i - 1]) * (MapWidth / 2.f) + Offset,
					MiniMapPoints[i] * (MapHeight / 2.f) + Offset);
				DebugFrustumLine.SetColor(FLinearColor::Yellow);
				DebugFrustumLine.LineThickness = UIScale;
				Canvas->DrawItem(DebugFrustumLine);
			}
		}
	}
}

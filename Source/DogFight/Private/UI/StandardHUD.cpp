// Dog Fight Game Code By CYM.


#include "UI/StandardHUD.h"

#include "Game/StandardGameState.h"
#include "Player/StandardModePlayerController.h"
#include "MathHelper.h"
#include "DogFight.h"
#include "UI/Widget/GamePhaseMessageWidget.h"
#include "UI/Widget/OperationHintMessageWidget.h"
#include "UI/Widget/GameMessageWindowWidget.h"
#include "UI/Widget/GameRoundsTimelineWidget.h"
#include "UI/Widget/CardDisplayWidget.h"
#include "Game/StandardGameMode.h"
#include "Game/GameWorkflow/StandardGameMode/StandardGameModePhaseDefine.h"
#include "UI/Widget/GameTitleMessageWidget.h"
#include "UI/Widget/InGameHudWidget.h"
#include "UI/Widget/AbilityPanelWidget.h"
#include "UI/Widget/AbilitySelectWindowWidget.h"

AStandardHUD::AStandardHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MiniMapMargin = 40;
	MiniMapPoints.Init(FVector2D::ZeroVector, 4);
}

void AStandardHUD::DisplayHintMessage(FName HintMessage, TArray<FText> MessageArguments)
{
	if (OperationHintMessageWidget != nullptr)
	{
		if (HintMessage.IsNone())
		{
			HideOperationHintMessageWidget();
		}
		else
		{
			if (!OperationHintMessageWidget->IsInViewport())
			{
				InGameHudWidget->AddWidgetToSlotByName(OperationHintMessageWidget);
			}
			OperationHintMessageWidget->SetHintMessage(HintMessage, MessageArguments);
		}
	}
}

void AStandardHUD::HideOperationHintMessageWidget()
{
	if (OperationHintMessageWidget != nullptr)
	{
		if (OperationHintMessageWidget->Slot != nullptr)
		{
			OperationHintMessageWidget->RemoveFromParent();
		}
	}
}

void AStandardHUD::ShowGameMessage(FGameMessage Message)
{
	if (GameMessageWindowWidget != nullptr)
	{
		if (GameMessageWindowWidget->Slot == nullptr)
		{
			InGameHudWidget->AddWidgetToSlotByName(GameMessageWindowWidget);
		}
		
		GameMessageWindowWidget->AddGameMessage(Message);
	}
}

void AStandardHUD::InitializeTimelineDisplayTimeline()
{
	if (GameRoundsTimelineWidget != nullptr)
	{
		GameRoundsTimelineWidget->SetupTimelineDisplay();
	}
}

void AStandardHUD::SetCardDisplayInfoList(const TArray<FCardInstanceDisplayInfo>& CardInfoList)
{
	if (CardDisplayWidget != nullptr)
	{
		CardDisplayWidget->SetCardDisplayInfos(CardInfoList);
	}
}

void AStandardHUD::RefreshCardUsingAbilityInfo()
{
	if (CardDisplayWidget != nullptr)
	{
		CardDisplayWidget->OnCardUsingAbilityInfoChanged();
	}
}

void AStandardHUD::RefreshCardUsableState()
{
	if (CardDisplayWidget != nullptr)
	{
		CardDisplayWidget->OnCardUsableIndexChanged();
	}
}

void AStandardHUD::ToggleCardDisplayWidgetVisibility(bool bVisible)
{
	if (CardDisplayWidget == nullptr)
		return;

	if (bVisible && CardDisplayWidget->Slot == nullptr)
	{
		InGameHudWidget->AddWidgetToSlotByName(CardDisplayWidget);
	}
	else if (!bVisible && CardDisplayWidget->Slot != nullptr)
	{
		CardDisplayWidget->RemoveFromParent();
	}
}

void AStandardHUD::SetCardSelectionMode(ECardSelectionMode SelectionMode)
{
	if (CardDisplayWidget != nullptr)
	{
		CardDisplayWidget->SetSelectMode(SelectionMode);
	}
}

void AStandardHUD::SetCardDisplayWidgetSelectable(bool bSelectable)
{
	if (CardDisplayWidget != nullptr)
	{
		CardDisplayWidget->SetCardItemsSelectable(bSelectable);
	}
}


void AStandardHUD::SetTimelineVisibility(bool bVisible)
{
	if (GameRoundsTimelineWidget != nullptr)
	{
		if (bVisible && GameRoundsTimelineWidget->Slot == nullptr)
		{
			InGameHudWidget->AddWidgetToSlotByName(GameRoundsTimelineWidget);
		}
		else if (!bVisible && GameRoundsTimelineWidget->Slot != nullptr)
		{
			GameRoundsTimelineWidget->RemoveFromParent();
		}
	}
}

void AStandardHUD::ShowGameTitleMessage(FGameTitleMessage Message)
{
	if (GameTitleMessageWidget != nullptr)
	{
		if (GameTitleMessageWidget->Slot == nullptr)
		{
			InGameHudWidget->AddWidgetToSlotByName(GameTitleMessageWidget);
		}

		GameTitleMessageWidget->DisplayTitleMessage(Message);
	}
}

void AStandardHUD::StartDiscardCards(int32 CountToDiscard)
{
	if (CardDisplayWidget != nullptr)
	{
		if (CardDisplayWidget->Slot == nullptr)
		{
			InGameHudWidget->AddWidgetToSlotByName(CardDisplayWidget);
		}

		CardDisplayWidget->SetDesireSelectCount(CountToDiscard);
		CardDisplayWidget->SetSelectMode(ECardSelectionMode::CSM_MultiWithConfirm);
	}
}

void AStandardHUD::StopDiscardCards()
{
	if (CardDisplayWidget != nullptr)
	{
		CardDisplayWidget->SetSelectMode(ECardSelectionMode::CSM_SingleNoConfirm);
	}
}

void AStandardHUD::StartRequestResponseCard(int32 MaxCardCount)
{
	if (CardDisplayWidget != nullptr)
	{
		if (CardDisplayWidget->Slot == nullptr)
		{
			InGameHudWidget->AddWidgetToSlotByName(CardDisplayWidget);
		}

		CardDisplayWidget->SetDesireSelectCount(MaxCardCount);
		CardDisplayWidget->SetSelectMode(ECardSelectionMode::CSM_AnyWithConfirm);
	}
}

void AStandardHUD::AddNewPlayerAbility(FAbilityDisplayInfo AbilityInfo, int32 AbilitySlot)
{
	if (AbilityPanelWidget != nullptr)
	{
		if (AbilityPanelWidget->Slot == nullptr)
		{
			InGameHudWidget->AddWidgetToSlotByName(AbilityPanelWidget);
		}

		AbilityPanelWidget->AddAbility(AbilityInfo, AbilitySlot);
	}
}

void AStandardHUD::RemovePlayerAbility(int32 AbilitySlot)
{
	if (AbilityPanelWidget != nullptr)
	{
		AbilityPanelWidget->RemoveAbility(AbilitySlot);
	}
}

void AStandardHUD::UpdateAbilityCooldown(int32 AbilitySlot, int32 CurrentCooldown)
{
	if (AbilityPanelWidget != nullptr)
	{
		AbilityPanelWidget->UpdateAbilityCooldownAtSlot(AbilitySlot, CurrentCooldown);
	}
}

void AStandardHUD::UpdateAbilityAvailability(int32 AbilitySlot, bool NewAvailability)
{
	if (AbilityPanelWidget != nullptr)
	{
		AbilityPanelWidget->UpdateAbilityAvailability(AbilitySlot, NewAvailability);
	}
}

void AStandardHUD::ShowAbilitySelectWindow(TArray<FAbilityDisplayInfo> AbilityDisplayInfos)
{
	if (AbilitySelectWindowWidget != nullptr)
	{
		if (AbilitySelectWindowWidget->Slot == nullptr)
		{
			InGameHudWidget->AddWidgetToSlotByName(AbilitySelectWindowWidget);
		}

		AbilitySelectWindowWidget->SetDisplayAbilities(AbilityDisplayInfos);
	}
}

void AStandardHUD::HideAbilitySelectWindow()
{
	if (AbilitySelectWindowWidget != nullptr)
	{
		if (AbilitySelectWindowWidget->Slot != nullptr)
		{
			AbilitySelectWindowWidget->RemoveFromParent();
		}
	}
}

void AStandardHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();

	if (InGameHudWidget == nullptr && InGameHudWidgetClass != nullptr)
	{
		InGameHudWidget = CreateWidget<UInGameHudWidget>(PlayerController, InGameHudWidgetClass, FName("InGameHudWidget"));
		InGameHudWidget->AddToViewport();
	}

	// Instantiate GamePhaseMessageWidget
	if (GamePhaseMessageWidget == nullptr && GamePhaseMessageWidgetClass != nullptr)
	{
		GamePhaseMessageWidget = CreateWidget<UGamePhaseMessageWidget>(PlayerController, GamePhaseMessageWidgetClass, FName("GamePhaseMessageWidget"));
	}

	if (OperationHintMessageWidget == nullptr && OperationHintMessageWidgetClass != nullptr)
	{
		OperationHintMessageWidget = CreateWidget<UOperationHintMessageWidget>(PlayerController, OperationHintMessageWidgetClass, FName("OperationHintMessageWidget"));
	}

	if (GameMessageWindowWidget == nullptr && GameMessageWindowWidgetClass != nullptr)
	{
		GameMessageWindowWidget = CreateWidget<UGameMessageWindowWidget>(PlayerController, GameMessageWindowWidgetClass, FName("GameMessageWindowWidget"));
	}

	if (GameRoundsTimelineWidget == nullptr && GameRoundsTimelineWidgetClass != nullptr)
	{
		GameRoundsTimelineWidget = CreateWidget<UGameRoundsTimelineWidget>(PlayerController, GameRoundsTimelineWidgetClass, FName("GameRoundsTimelineWidget"));
	}

	if (CardDisplayWidget == nullptr && CardDisplayWidgetClass != nullptr)
	{
		CardDisplayWidget = CreateWidget<UCardDisplayWidget>(PlayerController, CardDisplayWidgetClass, FName("CardDisplayWidget"));
	}

	if (GameTitleMessageWidget == nullptr && GameTitleMessageWidgetClass != nullptr)
	{
		GameTitleMessageWidget = CreateWidget<UGameTitleMessageWidget>(PlayerController, GameTitleMessageWidgetClass, FName("GameTitleMessageWidget"));
	}

	if (AbilityPanelWidget == nullptr && AbilityPanelWidgetClass != nullptr)
	{
		AbilityPanelWidget = CreateWidget<UAbilityPanelWidget>(PlayerController, AbilityPanelWidgetClass, FName("AbilityPanelWidget"));
	}

	if (AbilitySelectWindowWidget == nullptr && AbilitySelectWindowWidgetClass != nullptr)
	{
		AbilitySelectWindowWidget = CreateWidget<UAbilitySelectWindowWidget>(PlayerController, AbilitySelectWindowWidgetClass, FName("AbilitySelectWindowWidget"));
	}

	// Listen the GamePhase change event
	StandardGameState = GetWorld()->GetGameState<AStandardGameState>();
	if (StandardGameState != nullptr)
	{
		StandardGameState->OnGamePhaseChanged.AddDynamic(this, &AStandardHUD::OnGamePhaseChanged);
		StandardGameState->OnCountdownContentStringChanged.AddDynamic(this, &AStandardHUD::OnCountdownContentStringChanged);

		// Set the initial display content
		if (GamePhaseMessageWidget != nullptr)
		{
			GamePhaseMessageWidget->SetGamePhase(StandardGameState->GetCurrentGamePhase());
			GamePhaseMessageWidget->SetCountdownContent(StandardGameState->GetCountdownContentString());
		}

		// Broadcast current phase
		OnGamePhaseChanged(StandardGameState->GetCurrentGamePhase());
	}
}

bool AStandardHUD::ShouldPhaseMessageDisplay(FName GamePhase) const
{
	return GamePhase == StandardGameModePhase::EnteringMap || GamePhase == StandardGameModePhase::WaitingForStart || GamePhase == StandardGameModePhase::FreeMoving;
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

void AStandardHUD::OnGamePhaseChanged(const FName& NewGamePhase)
{
	if (GamePhaseMessageWidget == nullptr)
		return;

	if (ShouldPhaseMessageDisplay(NewGamePhase))
	{
		GamePhaseMessageWidget->SetGamePhase(NewGamePhase);
		GamePhaseMessageWidget->SetCountdownContent(StandardGameState->GetCountdownContentString());
		// Add to viewport if not be added yet
		if (GamePhaseMessageWidget->Slot == nullptr)
		{
			InGameHudWidget->AddWidgetToSlotByName(GamePhaseMessageWidget);
		}
	}
	else
	{
		GamePhaseMessageWidget->RemoveFromParent();
	}
}

void AStandardHUD::OnCountdownContentStringChanged()
{
	if (GamePhaseMessageWidget == nullptr)
		return;

	GamePhaseMessageWidget->SetCountdownContent(StandardGameState->GetCountdownContentString());
}

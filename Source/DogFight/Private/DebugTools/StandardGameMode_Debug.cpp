#include "AI/StandardModeAIController.h"
#include "Card/CardBase.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardPlayerState.h"
#include "Game/GameWorkflow/GamePhaseCommon.h"
#include "Pawns/StandardModePlayerCharacter.h"
#include "Player/StandardModePlayerController.h"

#if WITH_IMGUI

#define MAIN_WINDOW_BASE_INFO		0
#define MAIN_WINDOW_RELATION_INFO	1
#define MAIN_WINDOW_CARD_MANAGE		2

FDebugPlayerRelationInfo::FDebugPlayerRelationInfo(FPlayerRelationStatistic& PlayerRelationStatistic)
{
	PlayerId = PlayerRelationStatistic.PlayerId;
	PlayerName = "";
	RelationPoint = PlayerRelationStatistic.RelationPoint;
	ReceiveDamage = PlayerRelationStatistic.ReceiveDamage;
	CurrentHealth = PlayerRelationStatistic.CurrentHealth;
}

void AStandardGameMode::ImGuiTick()
{
	if (!bShowDebugTools)
		return;

	ImGui::SetNextWindowSize(ImVec2(550, 400), ImGuiCond_FirstUseEver);

	ImGuiWindowFlags WindowFlags = 0;
	WindowFlags |= ImGuiWindowFlags_MenuBar;

	if (!ImGui::Begin("GameMode Admin", nullptr, WindowFlags))
	{
		// Return if the window is collapsed
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
	// Main window menu
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Debuggers"))
		{
			ImGui::MenuItem("State Machine Debugger", nullptr, &bShowStateMachineDebugger);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::AlignTextToFramePadding();
	ImGui::Text("This is GameMode administration page.");
	ImGui::SameLine();
	if (ImGui::Button("Close"))
	{
		ToggleGameModeAdmin();
	}

	if (ImGui::BeginTabBar("MainWindowTabBar", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("PlayerBaseInfo"))
		{
			MainWindowTabIndex = MAIN_WINDOW_BASE_INFO;
			DrawPlayerBaseInfoTab();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("PlayerRelationInfo"))
		{
			MainWindowTabIndex = MAIN_WINDOW_RELATION_INFO;
			DrawPlayerRelationInfoTab();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("PlayerCardManage"))
		{
			MainWindowTabIndex = MAIN_WINDOW_CARD_MANAGE;
			DrawCardManageTab();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	// Draw state machine debugger
	if (bShowStateMachineDebugger)
	{
		DrawStateMachineDebugger(&bShowStateMachineDebugger);
	}

	ImGui::End();
}

void AStandardGameMode::DrawPlayerBaseInfoTab()
{
	ImGui::Columns(6, "PlayerBaseInfo");
	ImGui::Separator();
	// Draw header
	ImGui::Text("ID");
	ImGui::NextColumn();
	ImGui::Text("Name");
	ImGui::NextColumn();
	ImGui::Text("Cards");
	ImGui::NextColumn();
	ImGui::Text("CardUsed");
	ImGui::NextColumn();
	ImGui::Text("Health");
	ImGui::NextColumn();
	ImGui::Text("Strength");
	ImGui::NextColumn();
	// Fill in all player info
	TArray<FDebugPlayerBaseInfo> PlayerBaseInfos;
	PlayerBaseInfoMap.GenerateValueArray(PlayerBaseInfos);
	for (auto Record : PlayerBaseInfoMap)
	{
		FDebugPlayerBaseInfo& PlayerBaseInfo = Record.Value;
		if (PlayerBaseInfo.bActive)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
		}
		char IdLabel[32];
		sprintf_s(IdLabel, "%03d", PlayerBaseInfo.PlayerId);
		if (ImGui::Selectable(IdLabel, PlayerIdShowBaseInfo == PlayerBaseInfo.PlayerId, ImGuiSelectableFlags_SpanAllColumns))
		{
			PlayerIdShowBaseInfo = PlayerBaseInfo.PlayerId;
		}
		ImGui::NextColumn();
		ImGui::Text("%ls", *PlayerBaseInfo.PlayerName);
		ImGui::NextColumn();
		ImGui::Text("%d/%d", PlayerBaseInfo.HoldCards, PlayerBaseInfo.MaxHoldCards);
		ImGui::NextColumn();
		ImGui::Text("%d/%d", PlayerBaseInfo.UsedCards, PlayerBaseInfo.MaxUseCards);
		ImGui::NextColumn();
		ImGui::Text("%d/%d", PlayerBaseInfo.CurrentHealth, PlayerBaseInfo.MaxHealth);
		ImGui::NextColumn();
		ImGui::Text("%d/%d", PlayerBaseInfo.CurrentStrength, PlayerBaseInfo.MaxStrength);
		ImGui::NextColumn();
		if (PlayerBaseInfo.bActive)
		{
			ImGui::PopStyleColor();
		}
	}
	ImGui::Separator();
	ImGui::Columns(1);
}

void AStandardGameMode::DrawPlayerRelationInfoTab()
{
	// Construct player selecting list
	ImGui::BeginGroup();
	{
		ImGui::Text("Player List:");
		if (ImGui::ListBoxHeader("##PlayerRelationInfoPlayerSelect", ImVec2(120,400)))
		{
			TArray<int32> PlayerIdList;
			PlayerBaseInfoMap.GetKeys(PlayerIdList);
			for (int32 PlayerId : PlayerIdList)
			{
				bool bSelected = (PlayerId == PlayerIdShowRelationship);
				char SelectableLabel[32];
				sprintf_s(SelectableLabel, "ID [%d]", PlayerId);

				if (ImGui::Selectable(SelectableLabel, bSelected))
				{
					PlayerIdShowRelationship = PlayerId;
					GatherRelationshipInfo(PlayerId);
				}

				if (bSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::ListBoxFooter();
		}
	}
	ImGui::EndGroup();
	ImGui::SameLine();

	ImGui::BeginGroup();
	if (PlayerBaseInfoMap.Contains(PlayerIdShowRelationship))
	{
		ImGui::Text("Relationship record of [%ls]", *GetPlayerNameById(PlayerIdShowRelationship));
		// Draw relation data
		ImGui::Separator();
		ImGui::Columns(5);
		// Draw header
		ImGui::Text("ID");
		ImGui::NextColumn();
		ImGui::Text("Name");
		ImGui::NextColumn();
		ImGui::Text("RelationPoint");
		ImGui::NextColumn();
		ImGui::Text("ReceiveDamage");
		ImGui::NextColumn();
		ImGui::Text("CurrentHealth");
		ImGui::NextColumn();
		// Draw data
		for (int32 Index = 0; Index < ShowingPlayerRelationships.Num(); ++Index)
		{
			char IdLabel[32];
			sprintf_s(IdLabel, "%03d", ShowingPlayerRelationships[Index].PlayerId);
			ImGui::Selectable(IdLabel, false, ImGuiSelectableFlags_SpanAllColumns);
			ImGui::NextColumn();
			ImGui::Text("%ls", *ShowingPlayerRelationships[Index].PlayerName);
			ImGui::NextColumn();
			ImGui::Text("%d", ShowingPlayerRelationships[Index].RelationPoint);
			ImGui::NextColumn();
			ImGui::Text("%d", ShowingPlayerRelationships[Index].ReceiveDamage);
			ImGui::NextColumn();
			ImGui::Text("%d", ShowingPlayerRelationships[Index].CurrentHealth);
			ImGui::NextColumn();
		}
		ImGui::Separator();
		ImGui::Columns(1);
	}
	else
	{
		ImGui::Text("Invalid player id [%d] to check relationship.", PlayerIdShowRelationship);
	}
	ImGui::EndGroup();
}

void AStandardGameMode::DrawCardManageTab()
{
	// Construct player selecting list
	ImGui::BeginGroup();
	{
		ImGui::Text("Player List:");
		if (ImGui::ListBoxHeader("##CardManagePlayerSelect", ImVec2(120,400)))
		{
			TArray<int32> PlayerIdList;
			PlayerBaseInfoMap.GetKeys(PlayerIdList);
			for (int32 PlayerId : PlayerIdList)
			{
				bool bSelected = (PlayerId == PlayerIdManageCards);
				char SelectableLabel[32];
				sprintf_s(SelectableLabel, "ID [%d]", PlayerId);

				if (ImGui::Selectable(SelectableLabel, bSelected))
				{
					PlayerIdManageCards = PlayerId;
					GatherCardDisplayInfo(PlayerId);
				}

				if (bSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::ListBoxFooter();
		}
	}
	ImGui::EndGroup();
	ImGui::SameLine();

	// Draw card list
	ImGui::BeginGroup();
	{
		ImGui::Text("Card List:");
		if (ImGui::ListBoxHeader("##CardManageCardSelect", ImVec2(150, 400)))
		{
			for (int32 Index = 0; Index < SelectedPlayerCardInfoList.Num(); ++Index)
			{
				FDebugCardDisplayInfo& CurrentInfo = SelectedPlayerCardInfoList[Index];
				bool bSelected = Index == CardIndexShowDetails;
				char CardItemLabel[32];
				sprintf_s(CardItemLabel, "%ls##%d", *CurrentInfo.CardClassName, Index);
				if (ImGui::Selectable(CardItemLabel, bSelected))
				{
					CardIndexShowDetails = Index;
				}
				if (bSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			// Make select card index invalid if no card to show
			if (SelectedPlayerCardInfoList.Num() == 0)
			{
				CardIndexShowDetails = -1;
			}
			// Shrink the index by list length
			else if (CardIndexShowDetails >= SelectedPlayerCardInfoList.Num())
			{
				CardIndexShowDetails = SelectedPlayerCardInfoList.Num() - 1;
			}
			ImGui::ListBoxFooter();
		}
	}
	ImGui::EndGroup();
	ImGui::SameLine();

	// Draw card detail panel
	ImGui::BeginGroup();
	{
		if (CardIndexShowDetails != -1)
		{
			FDebugCardDisplayInfo& CurrentCardInfo = SelectedPlayerCardInfoList[CardIndexShowDetails];
			ImGui::Text("Detail:");
			ImGui::Columns(2);
			ImGui::Separator();
			// Card Details
			ImGui::Text("CardClass:");
			ImGui::NextColumn();
			ImGui::Text("%ls", *CurrentCardInfo.CardClassName);
			ImGui::NextColumn();

			for (int32 i = 0; i < CurrentCardInfo.InstructionList.Num(); ++i)
			{
				if (i == 0)
				{
					ImGui::Text("Instruction:");
					ImGui::NextColumn();
				}
				else
				{
					ImGui::Text("");
					ImGui::NextColumn();
				}
				ImGui::Text("%ls", *CurrentCardInfo.InstructionList[i]);
				ImGui::NextColumn();
			}

			ImGui::Separator();
			ImGui::Columns(1);
		}
		else
		{
			ImGui::Text("Select any card to check detail.");
		}
	}
	ImGui::EndGroup();
}

void AStandardGameMode::DrawStateMachineDebugger(bool* bOpen)
{
	ImGui::SetNextWindowSize(ImVec2(340, 200), ImGuiCond_FirstUseEver);
	ImGui::Begin("State Machine Debugger", bOpen, ImGuiWindowFlags_None);

	// Game phase history box
	ImGui::BeginGroup();
	{
		ImGui::Text("Game Phase History:");
		if (ImGui::ListBoxHeader("##PhaseHistory", ImVec2(200, 200)))
		{
			for (int32 Index = 0; Index < StateMachineGamePhaseHistory.Num(); ++Index)
			{
				FDebugGamePhaseHistoryRecord& Record = StateMachineGamePhaseHistory[Index];
				char RecordLabel[32];
				sprintf_s(RecordLabel, "[%05d] %ls", Index, *Record.GamePhaseName.ToString());
				if (ImGui::Selectable(RecordLabel, GamePhaseHistorySelectIndex == Index))
				{
					GamePhaseHistorySelectIndex = Index;

					bWasLastGamePhaseSelected = (Index == StateMachineGamePhaseHistory.Num() - 1);
				}
			}

			if (CachedGamePhaseHistoryCount < StateMachineGamePhaseHistory.Num())
			{
				// Follow new create item if last one was selected
				if (bWasLastGamePhaseSelected)
				{
					GamePhaseHistorySelectIndex = CachedGamePhaseHistoryCount;
					ImGui::SetItemDefaultFocus();
					ImGui::SetScrollHereY();
				}
				CachedGamePhaseHistoryCount = StateMachineGamePhaseHistory.Num();
			}
			ImGui::ListBoxFooter();
		}
	}
	ImGui::EndGroup();
	ImGui::SameLine();

	// Game phase detail box
	if (GamePhaseHistorySelectIndex < StateMachineGamePhaseHistory.Num())
	{
		FDebugGamePhaseHistoryRecord& Record = StateMachineGamePhaseHistory[GamePhaseHistorySelectIndex];
		ImGui::BeginGroup();
		{

			ImGui::Columns(2);
			// Phase name
			ImGui::Text("Game Phase:");
			ImGui::NextColumn();
			ImGui::Text("%ls", *Record.GamePhaseName.ToString());
			ImGui::NextColumn();
			// Owner player
			ImGui::Text("Owner Player:");
			ImGui::NextColumn();
			ImGui::Text("%ls [ID:%d]", *GetPlayerNameById(Record.PlayerId), Record.PlayerId);
			ImGui::NextColumn();
			// Time point
			ImGui::Text("Time:");
			ImGui::NextColumn();
			ImGui::Text("%d:%d", Record.TimeMinutes, Record.TimeSeconds);
			ImGui::NextColumn();
			ImGui::Columns(1);
		}

		// More details
		ImGui::Separator();
		if ((Record.SwitchMethod & EGamePhaseSwitchFlags::SF_Interrupted) > 0)
		{
			ImGui::Text("Interrupted %ls", *StateMachineGamePhaseHistory[GamePhaseHistorySelectIndex - 1].GamePhaseName.ToString());
		}
		if ((Record.SwitchMethod & EGamePhaseSwitchFlags::SF_Resumed) > 0)
		{
			ImGui::Text("Resumed");
		}

		ImGui::Text("Extra Events:");
		if (ImGui::ListBoxHeader("##ExtraEvents", ImVec2(ImGui::GetContentRegionAvailWidth(), 200)))
		{
			for (int32 Index = 0; Index < Record.ExtraEvents.Num(); ++Index)
			{
				ImGui::Text("%ls", *Record.ExtraEvents[Index]);
			}
			ImGui::ListBoxFooter();
		}
		ImGui::EndGroup();
	}
	else
	{
		ImGui::Text("No game phase selected.");
	}

	ImGui::End();
}

void AStandardGameMode::SetupDebugTools()
{
	ImGuiTickHandle = FImGuiModule::Get().AddWorldImGuiDelegate(FImGuiDelegate::CreateUObject(this, &AStandardGameMode::ImGuiTick));
}

void AStandardGameMode::RemoveDebugTools()
{
	if (ImGuiTickHandle.IsValid())
	{
		FImGuiModule::Get().RemoveImGuiDelegate(ImGuiTickHandle);
	}
}

void AStandardGameMode::GatherRelationshipInfo(int32 PlayerId)
{
	ShowingPlayerRelationships.Empty();

	if (IsHumanPlayer(PlayerId))
	{
		if (AStandardModePlayerController* PlayerController = GetPlayerControllerById(PlayerId))
		{
			if (AStandardPlayerState* PlayerState = PlayerController->GetPlayerState<AStandardPlayerState>())
			{
				for (FPlayerRelationStatistic Record : PlayerState->GetPlayerRelationStatisticList())
				{
					FDebugPlayerRelationInfo NewRelationInfo(Record);
					NewRelationInfo.PlayerName = GetPlayerNameById(Record.PlayerId);

					ShowingPlayerRelationships.Add(NewRelationInfo);
				}
			}
		}
	}
	else
	{
		if (AStandardModeAIController* AIController = GetAIControllerById(PlayerId))
		{
			if (AStandardPlayerState* PlayerState = AIController->GetPlayerState<AStandardPlayerState>())
			{
				for (FPlayerRelationStatistic Record : PlayerState->GetPlayerRelationStatisticList())
				{
					FDebugPlayerRelationInfo NewRelationInfo(Record);
					NewRelationInfo.PlayerName = GetPlayerNameById(Record.PlayerId);

					ShowingPlayerRelationships.Add(NewRelationInfo);
				}
			}
		}
	}
}

void AStandardGameMode::GatherCardDisplayInfo(int32 PlayerId)
{
	SelectedPlayerCardInfoList.Empty();

	if (IsHumanPlayer(PlayerId))
	{
		if (AStandardModePlayerController* StandardModePlayerController = GetPlayerControllerById(PlayerId))
		{
			if (AStandardPlayerState* StandardPlayerState = StandardModePlayerController->GetPlayerState<AStandardPlayerState>())
			{
				TArray<ACardBase*> AllCards = StandardPlayerState->GetCardInstanceList();
				for (ACardBase* Card : AllCards)
				{
					FDebugCardDisplayInfo NewCardInfo;
					NewCardInfo.CardClassName = Card->GetClass()->GetName();
					TArray<UCardInstructionBase*> InstructionList = Card->GetAllInstruction();
					for (UCardInstructionBase* Instruction : InstructionList)
					{
						NewCardInfo.InstructionList.Add(Instruction->GetClass()->GetName());
					}

					SelectedPlayerCardInfoList.Add(NewCardInfo);
				}
			}
		}
	}
	else
	{
		if (AStandardModeAIController* StandardModeAIController = GetAIControllerById(PlayerId))
		{
			if (AStandardPlayerState* StandardPlayerState = StandardModeAIController->GetPlayerState<AStandardPlayerState>())
			{
				TArray<ACardBase*> AllCards = StandardPlayerState->GetCardInstanceList();
				for (ACardBase* Card : AllCards)
				{
					FDebugCardDisplayInfo NewCardInfo;
					NewCardInfo.CardClassName = Card->GetClass()->GetName();
					TArray<UCardInstructionBase*> InstructionList = Card->GetAllInstruction();
					for (UCardInstructionBase* Instruction : InstructionList)
					{
						NewCardInfo.InstructionList.Add(Instruction->GetClass()->GetName());
					}

					SelectedPlayerCardInfoList.Add(NewCardInfo);
				}
			}
		}
	}
}

FString AStandardGameMode::GetPlayerNameById(int32 PlayerId)
{
	if (PlayerBaseInfoMap.Contains(PlayerId))
	{
		return PlayerBaseInfoMap[PlayerId].PlayerName;
	}

	return "";
}

void FDebugGamePhaseHistoryRecord::AddSelectActorEvent(AActor* Actor, int32 EventPlayerId)
{
	if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(Actor))
	{
		if (APlayerState* PlayerState = StandardModePlayerCharacter->GetSupremeController()->GetPlayerState<APlayerState>())
		{
			ExtraEvents.Add(FString::Printf(TEXT("Player[%d] targeting actor: Player [%d]"), EventPlayerId, PlayerState->GetPlayerId()));
		}
	}
	else
	{
		ExtraEvents.Add(FString::Printf(TEXT("Player[%d] targeting actor: %s"), EventPlayerId, *Actor->GetName()));
	}
}

#endif

void AStandardGameMode::OnPlayerCardCountChanged(AStandardPlayerState* PlayerState)
{
#if WITH_IMGUI
	const int32 PlayerId = PlayerState->GetPlayerId();
	if (PlayerBaseInfoMap.Contains(PlayerId))
	{
		FDebugPlayerBaseInfo* PlayerBaseInfo = PlayerBaseInfoMap.Find(PlayerId);
		PlayerBaseInfo->HoldCards = PlayerState->GetCurrentCardCount();
		PlayerBaseInfo->MaxHoldCards = PlayerState->GetMaxCardNum();
		PlayerBaseInfo->UsedCards = PlayerState->GetUsedCardCount();
		PlayerBaseInfo->MaxUseCards = PlayerState->GetCardUseCountPerRound();
	}

	// Refresh card info
	if (MainWindowTabIndex == MAIN_WINDOW_CARD_MANAGE)
	{
		if (PlayerState->GetPlayerId() == PlayerIdManageCards)
		{
			GatherCardDisplayInfo(PlayerIdManageCards);
		}
	}
#endif
}

void AStandardGameMode::OnPlayerRelationInfoChanged()
{
#if WITH_IMGUI
	// Update the relation ship info
	if (MainWindowTabIndex == MAIN_WINDOW_RELATION_INFO)
	{
		GatherRelationshipInfo(PlayerIdShowRelationship);
	}
#endif
}

void AStandardGameMode::OnPlayerCardTargetAcquired(FCardInstructionTargetInfo& TargetInfo, int32 PlayerId)
{
#if WITH_IMGUI
	// Record into game phase history
	switch(TargetInfo.TargetType)
	{
	case ECardInstructionTargetType::Actor:
		GetCurrentGamePhaseRecord().AddSelectActorEvent(TargetInfo.ActorPtr, PlayerId);
		break;
	case ECardInstructionTargetType::Position:
		GetCurrentGamePhaseRecord().AddSelectPointEvent(TargetInfo.PositionValue, PlayerId);
		break;
	case ECardInstructionTargetType::Direction:
		GetCurrentGamePhaseRecord().AddSelectDirectionEvent(TargetInfo.DirectionValue, PlayerId);
		break;
	default: ;
	}
#endif
}


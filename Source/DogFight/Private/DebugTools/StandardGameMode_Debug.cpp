#include "AI/StandardModeAIController.h"
#include "Game/StandardGameMode.h"
#include "Game/StandardPlayerState.h"
#include "Player/StandardModePlayerController.h"

#if WITH_IMGUI

#define MAIN_WINDOW_BASE_INFO		0
#define MAIN_WINDOW_RELATION_INFO	1

FDebugPlayerRelationInfo::FDebugPlayerRelationInfo(FPlayerRelationStatistic PlayerRelationStatistic)
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

	if (!ImGui::Begin("GameMode Admin", NULL, ImGuiWindowFlags_None))
	{
		// Return if the window is collapsed
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
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
		ImGui::EndTabBar();
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
		sprintf(IdLabel, "%03d", PlayerBaseInfo.PlayerId);
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
		if (ImGui::ListBoxHeader("", ImVec2(120,400)))
		{
			TArray<int32> PlayerIdList;
			PlayerBaseInfoMap.GetKeys(PlayerIdList);
			for (int32 PlayerId : PlayerIdList)
			{
				bool bSelected = (PlayerId == PlayerIdShowRelationship);
				char SelectableLabel[32];
				sprintf(SelectableLabel, "ID [%d]", PlayerId);

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
			sprintf(IdLabel, "%03d", ShowingPlayerRelationships[Index].PlayerId);
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

FString AStandardGameMode::GetPlayerNameById(int32 PlayerId)
{
	if (PlayerBaseInfoMap.Contains(PlayerId))
	{
		return PlayerBaseInfoMap[PlayerId].PlayerName;
	}

	return "";
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

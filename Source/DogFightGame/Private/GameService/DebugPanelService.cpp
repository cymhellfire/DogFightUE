#include "GameService/DebugPanelService.h"
#include "ImGuiModule.h"
#include "Common/DogFightGameLog.h"

#define EXECUTE_TEXT_LENGTH 32

void FDebugPanelSameLineControl::DrawControl()
{
	ImGui::SameLine(0, Spacing);
}

void FDebugPanelTextControl::DrawControl()
{
	ImGui::Text(TCHAR_TO_ANSI(*Context));
}

void FDebugPanelButtonControl::DrawControl()
{
	if (ImGui::Button(TCHAR_TO_ANSI(*Context)))
	{
		ClickCallback.ExecuteIfBound();
	}
}

void FDebugPanelExecuteTextControl::DrawControl()
{
	static char InputBuff[EXECUTE_TEXT_LENGTH] = "";
	ImGui::InputText(TCHAR_TO_ANSI(*("##" + Context + "_Input")), InputBuff, EXECUTE_TEXT_LENGTH, ImGuiInputTextFlags_None);
	ImGui::SameLine();
	if (ImGui::Button(TCHAR_TO_ANSI(*(Context))))
	{
		ClickCallback.ExecuteIfBound(FString(ANSI_TO_TCHAR(InputBuff)));
	}
}

FDebugPanelExecuteComboBoxControl::FDebugPanelExecuteComboBoxControl()
	: SelectedIndex(-1)
{
}

FDebugPanelExecuteComboBoxControl::~FDebugPanelExecuteComboBoxControl()
{
	if (!EntryList.IsEmpty())
	{
		EntryList.Empty();
	}
}

void FDebugPanelExecuteComboBoxControl::DrawControl()
{
	ImGui::BeginGroup();
	{
		// Make list view
		if (ImGui::ListBoxHeader(TCHAR_TO_ANSI(*("##" + Context + "_Header")), ImVec2(ListSize.X, ListSize.Y)))
		{
			for (auto Entry : EntryList)
			{
				const bool bSelected = SelectedIndex == Entry.Index;

				if (ImGui::Selectable(TCHAR_TO_ANSI(*Entry.DisplayName), bSelected))
				{
					SelectedIndex = Entry.Index;
				}
			}
			ImGui::ListBoxFooter();
		}

		// Make button
		if (ImGui::Button(TCHAR_TO_ANSI(*Context)))
		{
			ClickCallback.ExecuteIfBound(SelectedIndex);
		}
	}
	ImGui::EndGroup();
}

UDebugPanelService::UDebugPanelService()
	: bDebugPanelEnable(false)
{
}

void UDebugPanelService::Startup()
{
	Super::Startup();

	// Use timer here to ensure the variable GWorld has the correct value for ImGui plugin.
	GetWorld()->GetTimerManager().SetTimer(DelayRegisterTimerHandle, this, &UDebugPanelService::OnDelayRegisterTimerExpired, 0.03f);
}

void UDebugPanelService::OnDelayRegisterTimerExpired()
{
	ImGuiTickHandle = FImGuiModule::Get().AddWorldImGuiDelegate(FImGuiDelegate::CreateUObject(this, &UDebugPanelService::ImGuiTick));

	// Gather all control settings
	GatherAllControlParams();
}

void UDebugPanelService::ToggleDebugPanel()
{
	bDebugPanelEnable = !bDebugPanelEnable;
	OnDebugPanelEnableChanged();
}

void UDebugPanelService::SetDrawingTab(const FName& InTabName)
{
	DrawingTabName = InTabName;
}

void UDebugPanelService::KeepSameLine(float Spacing, const FName& TabKey)
{
	TSharedPtr<FDebugPanelSameLineControl> SameLineControl = MakeShareable(new FDebugPanelSameLineControl);
	SameLineControl->Spacing = Spacing;
	SameLineControl->TabKey = GetActualTabName(TabKey);

	RecordNewControlParam(SameLineControl);
}

void UDebugPanelService::AddText(const FString& Context, const FName& TabKey)
{
	TSharedPtr<FDebugPanelTextControl> NewControlParam = MakeShareable(new FDebugPanelTextControl);
	NewControlParam->Context = Context;
	NewControlParam->TabKey = GetActualTabName(TabKey);

	// Record new control params
	RecordNewControlParam(NewControlParam);
}

void UDebugPanelService::AddButton(const FString& Context, FDebugPanelControlDelegate Callback, const FName& TabKey)
{
	TSharedPtr<FDebugPanelButtonControl> NewControlParam = MakeShareable(new FDebugPanelButtonControl);
	NewControlParam->Context = Context;
	NewControlParam->ClickCallback = Callback;
	NewControlParam->TabKey = GetActualTabName(TabKey);

	// Record enw control params
	RecordNewControlParam(NewControlParam);
}

void UDebugPanelService::AddExecuteText(const FString& Context, FDebugPanelExecuteTextDelegate Callback,
	const FName& TabKey)
{
	TSharedPtr<FDebugPanelExecuteTextControl> NewControlParams = MakeShareable(new FDebugPanelExecuteTextControl);
	NewControlParams->Context = Context;
	NewControlParams->ClickCallback = Callback;
	NewControlParams->TabKey = GetActualTabName(TabKey);

	RecordNewControlParam(NewControlParams);
}

void UDebugPanelService::AddExecuteComboBox(const FString& Context, TArray<FDebugPanelComboBoxEntryInfo> EntryInfos,
	FDebugPanelExecuteComboBoxDelegate Callback, FVector2D Size, const FName& TabKey)
{
	TSharedPtr<FDebugPanelExecuteComboBoxControl> NewControlParam = MakeShareable(new FDebugPanelExecuteComboBoxControl);
	NewControlParam->Context = Context;
	NewControlParam->EntryList = EntryInfos;
	NewControlParam->ClickCallback = Callback;
	NewControlParam->ListSize = Size;
	NewControlParam->TabKey = GetActualTabName(TabKey);

	RecordNewControlParam(NewControlParam);
}

void UDebugPanelService::RecordNewControlParam(TSharedPtr<FDebugPanelControlBase> InParams)
{
	auto& TabMap = DebugPanelControlMap.FindOrAdd(InParams->TabKey);
	TabMap.Add(InParams);
}

void UDebugPanelService::Shutdown()
{
	Super::Shutdown();

	if (ImGuiTickHandle.IsValid())
	{
		FImGuiModule::Get().RemoveImGuiDelegate(ImGuiTickHandle);
	}
}

void UDebugPanelService::OnDebugPanelEnableChanged()
{
	// Switch the ImGui input
	FImGuiModule::Get().GetProperties().SetInputEnabled(bDebugPanelEnable);
}

void UDebugPanelService::ImGuiTick()
{
	if (bDebugPanelEnable == 0)
	{
		return;
	}

	ImGui::SetNextWindowSize(ImVec2(550, 400), ImGuiCond_FirstUseEver);
	ImGuiWindowFlags WindowFlags = 0;

	if (ImGui::Begin("Debug Panel", nullptr, WindowFlags))
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text("This is debug panel.");
		ImGui::SameLine();
		if (ImGui::Button("Close"))
		{
			ToggleDebugPanel();
		}

		if (ImGui::BeginTabBar("DebugPanelTabBar", ImGuiTabBarFlags_None))
		{
			TArray<FName> TabNameList;
			DebugPanelControlMap.GenerateKeyArray(TabNameList);
			for (auto TabName : TabNameList)
			{
				DrawTab(TabName);
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}

void UDebugPanelService::DrawTab(const FName& InTabName)
{
	if (auto TabMapPtr = DebugPanelControlMap.Find(InTabName))
	{
		if (ImGui::BeginTabItem(TCHAR_TO_ANSI(*InTabName.ToString())))
		{
			auto TabMap = *TabMapPtr;
			for (auto ControlParam : TabMap)
			{
				if (!ControlParam.IsValid())
				{
					continue;
				}

				ControlParam->DrawControl();
			}

			ImGui::EndTabItem();
		}
	}
	else
	{
		UE_LOG(LogDogFightGame, Error, TEXT("[DebugPanelService] No controls for tab: %s"), *InTabName.ToString());
	}
}

FName UDebugPanelService::GetActualTabName(const FName& InName) const
{
	static FName DefaultName("Default"); 
	if (InName == DefaultName)
	{
		return DrawingTabName;
	}

	return InName;
}

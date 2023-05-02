#include "GameService/DebugPanelService.h"
#include "ImGuiModule.h"

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
}

void UDebugPanelService::ToggleDebugPanel()
{
	bDebugPanelEnable = !bDebugPanelEnable;
	OnDebugPanelEnableChanged();
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
	WindowFlags |= ImGuiWindowFlags_MenuBar;

	if (ImGui::Begin("Debug Panel", nullptr, WindowFlags))
	{
		// Construct window menu
		if (ImGui::BeginMenuBar())
		{
			
			ImGui::EndMenuBar();
		}

		ImGui::AlignTextToFramePadding();
		ImGui::Text("This is debug panel.");
		ImGui::SameLine();
		if (ImGui::Button("Test"))
		{
			UE_LOG(LogTemp, Log, TEXT("Click button"));
		}
	}
	ImGui::End();
}

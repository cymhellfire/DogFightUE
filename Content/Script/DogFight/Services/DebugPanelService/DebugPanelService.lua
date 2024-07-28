---@class DebugPanelService : GameServiceBase, UDebugPanelService
local DebugPanelService = UnrealClass("DogFight.Services.GameServiceBase")
local DebugPanelCardTab = require "DogFight.Services.DebugPanelService.DebugPanelCardTab"
local DebugPanelGameFlowTab = require "DogFight.Services.DebugPanelService.DebugPanelGameFlowTab"

function DebugPanelService:GatherAllControlParams()
    DebugPanelCardTab(self)

    DebugPanelGameFlowTab(self)

    self:SetDrawingTab("NewTab")
    self:AddButton("NewButton", {self, self.OnButtonClick})
end

function DebugPanelService:OnButtonClick()
    
end

return DebugPanelService
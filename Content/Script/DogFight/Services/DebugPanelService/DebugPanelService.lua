---@class DebugPanelService : GameServiceBase, UDebugPanelService
local DebugPanelService = UnrealClass("DogFight.Services.GameServiceBase")
local DebugPanelCardTab = require "DogFight.Services.DebugPanelService.DebugPanelCardTab"

function DebugPanelService:GatherAllControlParams()
    DebugPanelCardTab(self)

    self:SetDrawingTab("NewTab")
    self:AddButton("NewButton", {self, self.OnButtonClick})
end

function DebugPanelService:OnButtonClick()
    
end

return DebugPanelService
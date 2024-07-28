---@param self DebugPanelService
local function CharacterMovementButtons(self)
    self:AddText("Character Movement")
    self:KeepSameLine(10)
    self:AddButton("Enable", {self, function()
        -- Add character movement input to all players
        GetGameService(self, GameServiceNameDef.GameInputService):MulticastAddInputMapping(UE.EInputMappingType.InputMapping_CharacterMove)
        
        -- Enable character movement
        UE.UCommonGameFlowFunctionLibrary.SetCharacterMoveEnableForAllPlayers(self, true)
    end})
    self:KeepSameLine(10)
    self:AddButton("Disable", {self, function()
        -- Disable character movement
        UE.UCommonGameFlowFunctionLibrary.SetCharacterMoveEnableForAllPlayers(self, false)

        -- Remove character movement input to all players
        GetGameService(self, GameServiceNameDef.GameInputService):MulticastRemoveInputMapping(UE.EInputMappingType.InputMapping_CharacterMove)
    end})
end

---@param self DebugPanelService
local function DebugPanelGameFlowTab(self)
    self:SetDrawingTab("GameFlow")
    CharacterMovementButtons(self)
end

return DebugPanelGameFlowTab
---@param self DebugPanelService
local function DebugPanelCardTab(self)
    self:SetDrawingTab("CardTab")
    self:AddText("Test text")
    self:KeepSameLine(20)
    self:AddExecuteText("Exec", {self, function(self, Param)
        if type(Param) == "string" then
            print("OnExecClick with " .. Param)
        else
            print("OnExecClick")
        end
    end})

    local EntryList = UE.TArray(UE.FDebugPanelComboBoxEntryInfo)
    for i = 1, 4 do
        local NewEntry = UE.FDebugPanelComboBoxEntryInfo()
        NewEntry.Index = i
        NewEntry.DisplayName = "Entry:" .. i
        EntryList:Add(NewEntry)
    end

    self:AddExecuteComboBox("ComboBox", EntryList, {self, function(self, Index)
        local SelectEntry = EntryList:Get(Index)
        if SelectEntry then
            print("ExeuteCombo with " .. SelectEntry.DisplayName)
        end 
    end}, UE.FVector2D(200, 200))
end

return DebugPanelCardTab
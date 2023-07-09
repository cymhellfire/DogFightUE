---@class LobbyMapItemView : BP_Widget_LobbyMapListItem_C
---@field Data UGameplayExperience
local LobbyMapItemView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local LobbyMapItemVM = require("DogFight.Widget.GameLobby.LobbyMapItem.LobbyMapItemVM")

function LobbyMapItemView:PostInitialized()
    local NewVM = InstantiateViewModel(LobbyMapItemVM)
    self:BindViewModel(NewVM, {
        {BindKey = "MapName",   UIKey = "MapName_Text",   DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "ModeName",  UIKey = "ModeName_Text",    DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "BG_Color",  UIKey = "BG_Border",        DataBinding = DataBinding.ColorAndOpacityBinding() },
    })

    self.NormalBGColor = UE.FLinearColor(1, 1, 1, 0.2)
    self.SelectBGColor = UE.FLinearColor(1, 1, 1, 0.6)
end

---@param self LobbyMapItemView
---@param InGameExperience UGameplayExperience
local function InitDisplay(self, InGameExperience)
    self.ViewModel.MapName = InGameExperience.Title
    self.ViewModel.ModeName = InGameExperience.Desc

    self:BP_OnItemSelectionChanged(false)
end

function LobbyMapItemView:OnListItemObjectSet(InObject)
    self.Data = InObject:GetData()
    if self.Data then
        InitDisplay(self, self.Data)
    end
end

function LobbyMapItemView:BP_OnItemSelectionChanged(Selected)
    self.ViewModel.BG_Color = Selected and self.SelectBGColor or self.NormalBGColor
end

return LobbyMapItemView
---@class LobbyBrowserListItemView : BP_Widget_LobbyBrowserListItem_C
---@field Data UCommonSession_SearchResult
local LobbyBrowserListItemView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local LobbyBrowserListItemVM = require("DogFight.Widget.MainMenu.LobbyBrowser.LobbyBrowserListItemVM")

function LobbyBrowserListItemView:PostInitialized()
    local NewVM = InstantiateViewModel(LobbyBrowserListItemVM)
    self:BindViewModel(NewVM, {
        {BindKey = "LobbyName",     UIKey = "LobbyName_TextBlock",   DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "PlayerNum",     UIKey = "PlayerNum_TextBlock",   DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "BG_Color",      UIKey = "Item_BG",               DataBinding = DataBinding.ColorAndOpacityBinding() },
    })

    self.NormalColor = UE.FLinearColor(1, 0.628776, 1, 0.628776)
    self.SelectColor = UE.FLinearColor(1, 0.1, 1, 0.1)
end

function LobbyBrowserListItemView:OnListItemObjectSet(InObject)
    self.Data = InObject:GetData()
    if self.Data then
        local LobbyName, bFound = self.Data:GetStringSetting("LobbyName")
        self.ViewModel.LobbyName = bFound and LobbyName or self.Data:GetDescription()

        local TotalCount = self.Data:GetMaxPublicConnections()
        local AvailableCount = self.Data:GetNumOpenPublicConnections()
        self.ViewModel.PlayerNum = string.format("%d/%d", TotalCount - AvailableCount, TotalCount)

        self.ViewModel.BG_Color = self.NormalColor
    end
end

---@param bSelected boolean
function LobbyBrowserListItemView:BP_OnItemSelectionChanged(bSelected)
    self.ViewModel.BG_Color = bSelected and self.SelectColor or self.NormalColor

    
end

return LobbyBrowserListItemView
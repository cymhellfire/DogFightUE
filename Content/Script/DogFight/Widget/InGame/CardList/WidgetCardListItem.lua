require "UnLua"

local WidgetCardListItem = Class("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local WidgetCardListItemVM = require("DogFight.Widget.InGame.CardList.WidgetCardListItemVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")

function WidgetCardListItem:Initialize()
    local NewVM = InstantiateViewModel(WidgetCardListItemVM)
    self:BindViewModel(NewVM, {
        {BindKey = "CardName",          UIKey = "CardName_Text",    DataBinding = DataBinding.TextContextBinding(), },
        {BindKey = "BackgroundColor",   UIKey = "BG_Image",         DataBinding = DataBinding.ColorAndOpacityBinding(), },
    })

    ---@type FLinearColor
    self.NormalColor = UE.FLinearColor(1, 1, 1, 0.3)
    ---@type FLinearColor
    self.SelectColor = UE.FLinearColor(1, 1, 1, 0.7)
end

function WidgetCardListItem:OnListItemObjectSet(InObject)
    self.Data = InObject:GetData()
    if self.Data then
        self.ViewModel.CardName = self.Data.CardName
    end
end

function WidgetCardListItem:BP_OnItemSelectionChanged(bSelected)
    self.ViewModel.BackgroundColor = (bSelected and self.SelectColor or self.NormalColor)

    if bSelected and self.Data then
        local CardInstanceId = self.Data:GetCardInstanceId()
        print("use card with instance id: " .. CardInstanceId)
        UE.UCommonGameplayFunctionLibrary.UseCardByInstanceId(CardInstanceId)
    end
end

return WidgetCardListItem
require "UnLua"

local WidgetCardListItem = Class("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local WidgetCardListItemVM = require("DogFight.Widget.InGame.CardList.WidgetCardListItemVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")

function WidgetCardListItem:Initialize()
    local NewVM = InstantiateViewModel(WidgetCardListItemVM)
    self:BindViewModel(NewVM, {
        {BindKey = "CardName",   UIKey = "CardName_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })
end

function WidgetCardListItem:OnListItemObjectSet(InObject)
    local Data = InObject:GetData()
    if Data then
        self.ViewModel.CardName = Data.CardName
    end
end

return WidgetCardListItem
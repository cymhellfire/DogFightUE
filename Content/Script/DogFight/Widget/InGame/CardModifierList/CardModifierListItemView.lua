---@class CardModifierListItemView : BP_Widget_CardModifierListItem_C
local CardModifierListItemView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local CardModifierListItemVM = require("DogFight.Widget.InGame.CardModifierList.CardModifierListItemVM")

function CardModifierListItemView:PostInitialized()
    local NewVM = InstantiateViewModel(CardModifierListItemVM)
    self:BindViewModel(NewVM, {
        {BindKey = "Description",   UIKey = "CardModifierDesc_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })
end

function CardModifierListItemView:OnListItemObjectSet(InObject)
    ---@type UAttributeModifierDescObject
    self.Data = InObject:GetData()
    if self.Data then
        self.ViewModel.Description = self.Data:GetDescString()
    end
end

return CardModifierListItemView
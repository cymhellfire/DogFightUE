---@class PlayerCharacterAttributeItemView : ModelBase 
local PlayerCharacterAttributeItemView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local PlayerCharacterAttributeItemVM = require("DogFight.Widget.InGame.PlayerCharacterState.PlayerCharacterAttributeItemVM")
--local ListWrapper = require("Common.ListView.ListViewWrapper")

function PlayerCharacterAttributeItemView:Initialize()
    local NewVM = InstantiateViewModel(PlayerCharacterAttributeItemVM)
    self:BindViewModel(NewVM, {
        {BindKey = "AttributeName",     UIKey = "AttributeName_Text",       DataBinding = DataBinding.TextContextBinding(), },
        {BindKey = "AttributeValue",     UIKey = "AttributeValue_Text",       DataBinding = DataBinding.TextContextBinding(), },
    })
end

function PlayerCharacterAttributeItemView:PostInitialized()
    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)
end

function PlayerCharacterAttributeItemView:OnListItemObjectSet(InObject)
    ---@type UAttributeIntegerWrapperObject
    local Data = InObject:GetData()
    if Data then
        self.ViewModel.AttributeName = UE.UCommonWidgetFunctionLibrary.GetAttributeLocalizedName(Data)
        self.ViewModel.AttributeValue = tostring(Data:GetValue())
    end
end

return PlayerCharacterAttributeItemView
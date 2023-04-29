local WidgetCardListItem = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local WidgetCardListItemVM = require("DogFight.Widget.InGame.CardList.WidgetCardListItemVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")

function WidgetCardListItem:Initialize()
    local NewVM = InstantiateViewModel(WidgetCardListItemVM)
    self:BindViewModel(NewVM, {
        {BindKey = "CardName",          UIKey = "CardName_Text",    DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "CardDesc",          UIKey = "CardDesc_Text",    DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "BackgroundColor",   UIKey = "BG_Image",         DataBinding = DataBinding.ColorAndOpacityBinding() },
        {BindKey = "CardPicture",       UIKey = "CardPicture_Image",    DataBinding = DataBinding.TexturePathBinding(false) },
    })

    ---@type FLinearColor
    self.NormalColor = UE.FLinearColor(1, 1, 1, 0.3)
    ---@type FLinearColor
    self.SelectColor = UE.FLinearColor(1, 1, 1, 0.7)
    ---@type FLinearColor
    self.UsingColor = UE.FLinearColor(1, 1, 0, 0.8)
end

function WidgetCardListItem:Construct()
    self.bSelectable = true
    -- Register callback for card using events
    GetGameService(self, GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_MyCardBeginUsing, self, self.OnCardBeginUsing)
    GetGameService(self, GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_MyCardFinished, self, self.OnCardFinished)
    GetGameService(self, GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_MyCardCancelled, self, self.OnCardCancelled)
end

function WidgetCardListItem:Destruct()
    -- Unregister callback for card using events
    GetGameService(self, GameServiceNameDef.LuaEventService):UnregisterListener(UE.ELuaEvent.LuaEvent_MyCardBeginUsing, self, self.OnCardBeginUsing)
    GetGameService(self, GameServiceNameDef.LuaEventService):UnregisterListener(UE.ELuaEvent.LuaEvent_MyCardFinished, self, self.OnCardFinished)
    GetGameService(self, GameServiceNameDef.LuaEventService):UnregisterListener(UE.ELuaEvent.LuaEvent_MyCardCancelled, self, self.OnCardCancelled)
end

function WidgetCardListItem:OnListItemObjectSet(InObject)
    ---@type UCardDescObject
    self.Data = InObject:GetData()
    if self.Data then
        self.ViewModel.CardName = self.Data.CardName
        self.ViewModel.CardDesc = self.Data.CardDesc
        self.ViewModel.CardPicture = self.Data.CardPicturePath
    end
    self.ViewModel.BackgroundColor = self.NormalColor
end

---Update the background image color based on select state
---@param Item table ListItem to change color
---@param bSelected boolean Whether the item is selected
local function UpdateBackgroundColor(Item, bSelected)
    Item.ViewModel.BackgroundColor = (bSelected and Item.SelectColor or Item.NormalColor)
end

function WidgetCardListItem:BP_OnItemSelectionChanged(bSelected)
    UpdateBackgroundColor(self, bSelected)

    if bSelected and self.Data then
        local CardInstanceId = self.Data:GetCardInstanceId()
        print("use card with instance id: " .. CardInstanceId)
        UE.UCommonGameplayFunctionLibrary.UseCardByInstanceId(self, CardInstanceId)
    end
end

function WidgetCardListItem:OnCardBeginUsing(InId)
    -- Change visual if this card is using
    local CurInstanceId = self.Data:GetCardInstanceId()
    if CurInstanceId == InId then
        self.ViewModel.BackgroundColor = self.UsingColor
    end
end

function WidgetCardListItem:OnCardFinished(InId)

end

function WidgetCardListItem:OnCardCancelled(InId)
    UpdateBackgroundColor(self, false)
end

return WidgetCardListItem
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
    ---@type FLinearColor
    self.UsingColor = UE.FLinearColor(1, 1, 0, 0.8)
end

function WidgetCardListItem:Construct()
    self.bSelectable = true
    -- Register callback for card using events
    GetGameService(GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_MyCardBeginUsing, self, self.OnCardBeginUsing)
    GetGameService(GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_MyCardFinished, self, self.OnCardFinished)
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

function WidgetCardListItem:OnCardBeginUsing(InId)
    -- Change visual if this card is using
    local CurInstanceId = self.Data:GetCardInstanceId()
    if CurInstanceId == InId then
        self.ViewModel.BackgroundColor = self.UsingColor
    end
end

function WidgetCardListItem:OnCardFinished(InId)
    
end

return WidgetCardListItem
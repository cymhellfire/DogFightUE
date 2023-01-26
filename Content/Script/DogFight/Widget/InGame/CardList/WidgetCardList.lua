require "UnLua"

---@class WidgetCardList
---@field public CardList_ListView UListView
local WidgetCardList = Class("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local WidgetCardListVM = require("DogFight.Widget.InGame.CardList.WidgetCardListVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")

function WidgetCardList:Initialize()
    local NewVM = InstantiateViewModel(WidgetCardListVM)
    self:BindViewModel(NewVM, {
        --{BindKey = "TitleMessage",   UIKey = "TitleMessage_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })

    -- Cache local player id
    self.LocalPlayerId = UE.UCommonGameFlowFunctionLibrary.GetLocalPlayerId()
    print("Cache local player id: ".. self.LocalPlayerId)
    GetGameService(GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_CardListChanged, self, self.OnCardListChanged)
end

function WidgetCardList:PostInitialized()
    self.CardListWrapper = ListWrapper.New(self.CardList_ListView)
end

function WidgetCardList:OnCardListChanged(InPlayerId)
    if InPlayerId ~= self.LocalPlayerId then
        return
    end

    -- Get card list from 
    local PlayerState = UE.UCommonGameplayFunctionLibrary.GetPlayerStateById(self.LocalPlayerId)
    if PlayerState then
        local CardDescArray = PlayerState:GetAllCardDescObject()
        if CardDescArray:Length() > 0 then
            local CardDescTable = CardDescArray:ToTable()
            self.CardListWrapper:LoadDataByList(CardDescTable)
        end
    end
end

return WidgetCardList
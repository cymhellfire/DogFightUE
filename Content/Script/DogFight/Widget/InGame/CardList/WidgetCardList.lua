require "UnLua"

---@class WidgetCardList : BP_Widget_CardList_C
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
end

function WidgetCardList:PostInitialized()
    self.CardListWrapper = ListWrapper.New(self, self.CardList_ListView)

    -- Cache local player id
    self.LocalPlayerId = UE.UCommonGameFlowFunctionLibrary.GetLocalPlayerId(self)
    print("Cache local player id: ".. self.LocalPlayerId)
    GetGameService(self, GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_CardListChanged, self, self.OnCardListChanged)
end

function WidgetCardList:Construct()
    -- Register callback for card using events
    GetGameService(self, GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_MyCardBeginUsing, self, self.OnCardBeginUsing)
    GetGameService(self, GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_MyCardFinished, self, self.OnCardFinished)
    GetGameService(self, GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_MyCardCancelled, self, self.OnCardCancelled)
end

function WidgetCardList:Destruct()
    -- Unregister callback
    GetGameService(self, GameServiceNameDef.LuaEventService):UnregisterListener(UE.ELuaEvent.LuaEvent_MyCardBeginUsing, self, self.OnCardBeginUsing)
    GetGameService(self, GameServiceNameDef.LuaEventService):UnregisterListener(UE.ELuaEvent.LuaEvent_MyCardFinished, self, self.OnCardFinished)
    GetGameService(self, GameServiceNameDef.LuaEventService):UnregisterListener(UE.ELuaEvent.LuaEvent_MyCardCancelled, self, self.OnCardCancelled)
end

function WidgetCardList:OnCardListChanged(InPlayerId)
    if InPlayerId ~= self.LocalPlayerId then
        return
    end

    -- Get card list from 
    local PlayerState = UE.UCommonGameplayFunctionLibrary.GetPlayerStateById(self, self.LocalPlayerId)
    if PlayerState then
        local CardDescArray = PlayerState:GetAllCardDescObject()
        if CardDescArray:Length() > 0 then
            local CardDescTable = CardDescArray:ToTable()
            self.CardListWrapper:LoadDataByList(CardDescTable)
        elseif self.CardListWrapper then
            self.CardListWrapper:Clear()
        end
    end
end

function WidgetCardList:OnCardBeginUsing(InId)
    -- Disable card list selection
    self.CardList_ListView:SetSelectionMode(UE.ESelectionMode.None)
end

function WidgetCardList:OnCardCancelled(InId)
    -- Recover and reset card list selection
    self.CardList_ListView:SetSelectionMode(UE.ESelectionMode.Single)
    self.CardList_ListView:BP_ClearSelection()
end

function WidgetCardList:OnCardFinished(InId)
    -- Recover card list selection
    self.CardList_ListView:SetSelectionMode(UE.ESelectionMode.Single)
end

return WidgetCardList
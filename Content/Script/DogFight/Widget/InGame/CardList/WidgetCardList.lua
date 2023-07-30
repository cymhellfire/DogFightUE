---@class WidgetCardList : BP_Widget_CardList_C
---@field public CardList_ListView UListView
local WidgetCardList = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local WidgetCardListVM = require("DogFight.Widget.InGame.CardList.WidgetCardListVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")

function WidgetCardList:PostInitialized()
    local NewVM = InstantiateViewModel(WidgetCardListVM)
    self:BindViewModel(NewVM, {
        --{BindKey = "TitleMessage",   UIKey = "TitleMessage_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })

    self.CardList_ListView:SetSelectionMode(UE.ESelectionMode.None)
    self.CardListWrapper = ListWrapper.New(self, self.CardList_ListView)

    -- Cache local player id
    self.LocalPlayerId = UE.UCommonGameFlowFunctionLibrary.GetLocalPlayerId(self)
    print("Cache local player id: ".. self.LocalPlayerId)
    GetGameService(self, GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_CardListChanged, self, self.OnCardListChanged)
end

function WidgetCardList:Construct()
    -- Register callback for card using events
    ---@type LuaEventService
    local LuaEventService = GetGameService(self, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_MyCardBeginUsing, self, self.OnCardBeginUsing)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_MyCardFinished, self, self.OnCardFinished)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_MyCardCancelled, self, self.OnCardCancelled)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_StartPlayerRound, self, self.OnPlayerRoundStart)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_FinishPlayerRound, self, self.OnPlayerRoundFinish)
    end
end

function WidgetCardList:Destruct()
    -- Unregister callback
    ---@type LuaEventService
    local LuaEventService = GetGameService(self, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_MyCardBeginUsing, self, self.OnCardBeginUsing)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_MyCardFinished, self, self.OnCardFinished)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_MyCardCancelled, self, self.OnCardCancelled)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_StartPlayerRound, self, self.OnPlayerRoundStart)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_FinishPlayerRound, self, self.OnPlayerRoundFinish)
    end
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

function WidgetCardList:OnPlayerRoundStart(InPlayerId)
    -- Check if is local player round start
    local LocalPlayerId = UE.UCommonGameFlowFunctionLibrary.GetLocalPlayerId(self)
    if LocalPlayerId == InPlayerId then
        self.CardList_ListView:SetSelectionMode(UE.ESelectionMode.Single)
    end
end

function WidgetCardList:OnPlayerRoundFinish(InPlayerId)
    -- Check if is local player round finish
    local LocalPlayerId = UE.UCommonGameFlowFunctionLibrary.GetLocalPlayerId(self)
    if LocalPlayerId == InPlayerId then
        self.CardList_ListView:SetSelectionMode(UE.ESelectionMode.None)
    end
end

return WidgetCardList
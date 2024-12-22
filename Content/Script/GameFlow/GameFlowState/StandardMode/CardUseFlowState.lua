
---@class CardUseFlowState : GameFlowStateLogicBase State which is dedicated to create and execute CardUseFlow.
local CardUseFlowState =  UnrealClass("GameFlow.GameFlowState.GameFlowStateLogicBase")

function CardUseFlowState:OnEnter()
    print("CardUseFlowState: OnEnter")

    local bSucceed = false
    local ExtraData = self:GetExtraData()
    if ExtraData then
        local CardId = ExtraData.CardId

        ---@type ATopDownStylePlayerCharacter
        local CurCharacter = UE.UCommonGameFlowFunctionLibrary.GetCurrentTimelineEntityCharacter(self.OwnerState)
        ---@type UCharacterInventoryComponent
        local Inventory = CurCharacter and CurCharacter:GetInventoryComponent()
        if Inventory then
            ---@type UCard
            local CurCard = Inventory:GetCardByInstanceId(CardId)
            if CurCard then
                ---@type CardGameService
                local CardGameService = GetGameService(self.OwnerState, GameServiceNameDef.CardGameService)
                if CardGameService then
                    CardGameService:StartCardUseFlow(CurCard)
                    bSucceed = true
                    self.InitialCardId = CardId
                end
            else
                print("CardUseFlowState:OnEnter Failed to get card with id ", CardId)
            end
        else
            print("CardUseFlowState:OnEnter Failed to get inventory component.")
        end
    end

    if not bSucceed then
        print("CardUseFlowState:OnEnter Missing extra data.")
        self.OwnerState:Finish()
        return
    end

    -- Register listener
    ---@type LuaEventService
    local LuaEventService = GetGameService(self.OwnerState, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_PlayerCardAcquiredTarget, self, self.OnCardAcquiredTarget)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_CardUseFlowFinished, self, self.OnCardUseFlowFinished)
    end

    -- TODO: Iterate through timeline and ask for response
    self:ExecuteCardUseFlow()
end

function CardUseFlowState:OnExit()
    print("CardUseFlowState: OnExit")

    -- Unregister listener
    ---@type LuaEventService
    local LuaEventService = GetGameService(self.OwnerState, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_PlayerCardAcquiredTarget, self, self.OnCardAcquiredTarget)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_CardUseFlowFinished, self, self.OnCardUseFlowFinished)
    end
end

function CardUseFlowState:ExecuteCardUseFlow()
    print("CardUseFlowState: ExecuteCardUseFlow")

    ---@type CardGameService
    local CardGameService = GetGameService(self.OwnerState, GameServiceNameDef.CardGameService)
    if CardGameService then
        CardGameService:EndAndExecuteCardUseFlow()
    end
end

function CardUseFlowState:OnCardAcquiredTarget(InPlayerId, CardId)
    print("CardUseFlowState:OnCardAcquiredTarget ", InPlayerId, CardId)

    -- Get card from 
end

function CardUseFlowState:OnCardUseFlowFinished()
    print("CardUseFlowState:OnCardUseFlowFinished")

    self.OwnerState:Finish()
end

return CardUseFlowState
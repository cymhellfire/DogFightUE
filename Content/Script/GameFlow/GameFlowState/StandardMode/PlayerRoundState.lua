local BuffTypeDef = require "DogFight.Services.BuffService.BuffTypeDef"
local json = require "Common.json"

---@class PlayerRoundState : GameFlowStateLogicBase Player can use cards in this state.
local PlayerRoundState = UnrealClass("GameFlow.GameFlowState.GameFlowStateLogicBase")

function PlayerRoundState:OnEnter()
    print("PlayerRoundState: OnEnter")

    -- Record entity id
    self.CurEntityId = UE.UCommonGameFlowFunctionLibrary.GetCurrentTimelineEntityId(self.OwnerState)

    -- Listen to player card events
    ---@type LuaEventService
    local LuaEventService = GetGameService(self.OwnerState, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_PlayerCardAcquiredTarget, self, self.OnCardAcquiredTarget)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_PlayerCardFinished, self, self.OnCardFinished)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_FinishPlayerRound, self, self.OnPlayerFinished)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_OnPlayerCharacterDead, self, self.OnPlayerCharacterDead)
    end

    -- Add use card mapping to current player
    local CurPlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId(self.OwnerState)
    GetGameService(self.OwnerState, GameServiceNameDef.GameInputService):AddInputMappingByPlayerId(CurPlayerId, UE.EInputMappingType.InputMapping_CardUsing)

    -- Broadcast player round start event
    UE.UCommonGameFlowFunctionLibrary.BroadcastStartPlayerRound(self.OwnerState, CurPlayerId)
end

function PlayerRoundState:OnExit()
    print("PlayerRoundState: OnExit")

    -- Stop listen to player card events
    ---@type LuaEventService
    local LuaEventService = GetGameService(self.OwnerState, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_PlayerCardAcquiredTarget, self, self.OnCardAcquiredTarget)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_PlayerCardFinished, self, self.OnCardFinished)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_FinishPlayerRound, self, self.OnPlayerFinished)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_OnPlayerCharacterDead, self, self.OnPlayerCharacterDead)
    end

    -- Remove use card mapping from current player
    local CurPlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId(self.OwnerState)
    GetGameService(self.OwnerState, GameServiceNameDef.GameInputService):RemoveInputMappingByPlayerId(CurPlayerId, UE.EInputMappingType.InputMapping_CardUsing)

    -- Broadcast player round finish event
    UE.UCommonGameFlowFunctionLibrary.BroadcastFinishPlayerRound(self.OwnerState, CurPlayerId)
end

function PlayerRoundState:OnCardAcquiredTarget(InEntityId, InId)
    -- Skip if the triggered card is not used by current timeline entity
    if InEntityId ~= self.CurEntityId then
        return
    end

    -- Create CardUseFlow here
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(self.OwnerState, GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = "StandardMode.CardUseFlowState"
        NewArgument.Instigator = Instigator

        -- Add extra info
        local ExtraInfo = {
            CardId = InId,
        }
        NewArgument.ExtraData = json.encode(ExtraInfo)

        self.OwnerState:InsertState(NewArgument)
    end
end

function PlayerRoundState:OnCardFinished(InEntityId, InId)
    -- Skip if the finished card is not used by current timeline entity
    if InEntityId ~= self.CurEntityId then
        return
    end

    -- Check the remaining card count
    local CurCharacter = UE.UCommonGameFlowFunctionLibrary.GetCurrentTimelineEntityCharacter(self.OwnerState)
    local CardNum = CurCharacter and UE.UCommonGameplayFunctionLibrary.GetCharacterCardNums(CurCharacter) or 0
    print("PlayerRound: " .. CardNum .. " cards left")
    if CardNum <= 0 then
        self:FinishState()
    end
end

---@param InPlayerId number
function PlayerRoundState:OnPlayerFinished(InPlayerId)
    local CurPlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId(self.OwnerState)
    -- Skip if the event not triggered by current player
    if InPlayerId ~= CurPlayerId then
        return
    end

    self:FinishState()
end

---@param InPlayerId number
function PlayerRoundState:OnPlayerCharacterDead(InPlayerId)
    local CurPlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId(self.OwnerState)
    if InPlayerId == CurPlayerId then
        -- Finish round if current player is dead
        self:FinishState()
    else
        -- Check if only one player left
        local bShouldGameEnd = UE.UCommonGameplayFunctionLibrary.GetAlivePlayerNum(self.OwnerState) <= 1
        if bShouldGameEnd then
            self:FinishState()
        end
    end
end

function PlayerRoundState:FinishState()
    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(self.OwnerState, GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = "StandardMode.PostPlayerRoundCardState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
end

return PlayerRoundState
local BuffTypeDef = require "DogFight.Services.BuffService.BuffTypeDef"

---@class PlayerRoundState : GameFlowStateLogicBase Player can use cards in this state.
local PlayerRoundState = UnrealClass("GameFlow.GameFlowState.GameFlowStateLogicBase")

function PlayerRoundState:OnEnter()
    print("PlayerRoundState: OnEnter")

    -- Record player id
    self.CurPlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId(self.OwnerState)

    -- Listen to player card events
    ---@type LuaEventService
    local LuaEventService = GetGameService(self.OwnerState, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_PlayerCardFinished, self, self.OnCardFinished)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_FinishPlayerRound, self, self.OnPlayerFinished)
    end

    -- Add use card mapping to current player
    local CurPlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId(self.OwnerState)
    GetGameService(self.OwnerState, GameServiceNameDef.GameInputService):AddInputMappingByPlayerId(CurPlayerId, UE.EInputMappingType.InputMapping_CardUsing)
end

function PlayerRoundState:OnExit()
    print("PlayerRoundState: OnExit")

    -- Stop listen to player card events
    ---@type LuaEventService
    local LuaEventService = GetGameService(self.OwnerState, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_PlayerCardFinished, self, self.OnCardFinished)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_FinishPlayerRound, self, self.OnPlayerFinished)
    end

    -- Remove use card mapping from current player
    local CurPlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId(self.OwnerState)
    GetGameService(self.OwnerState, GameServiceNameDef.GameInputService):RemoveInputMappingByPlayerId(CurPlayerId, UE.EInputMappingType.InputMapping_CardUsing)
end

function PlayerRoundState:OnCardFinished(InPlayerId, InId)
    -- Skip if the finished card is not used by current player
    if InPlayerId ~= self.CurPlayerId then
        return
    end

    -- Check the remaining card count
    local CardNum = UE.UCommonGameplayFunctionLibrary.GetPlayerCardNums(self.OwnerState, self.CurPlayerId)
    print("PlayerRound: " .. CardNum .. " cards left")
    if CardNum <= 0 then
        self:FinishState()
    end
end

---@param InPlayerId number
function PlayerRoundState:OnPlayerFinished(InPlayerId)
    -- Skip if the event not triggered by current player
    if InPlayerId ~= self.CurPlayerId then
        return
    end

    self:FinishState()
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
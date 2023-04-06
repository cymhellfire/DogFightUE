require "UnLua"

---@class PlayerRoundState : GameFlowStateLogicBase Player can use cards in this state.
local PlayerRoundState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

function PlayerRoundState:OnEnter()
    print("PlayerRoundState: OnEnter")

    -- Record player id
    self.CurPlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId(self.OwnerState)

    -- Listen to player card events
    GetGameService(self.OwnerState, GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_PlayerCardFinished, self, self.OnCardFinished)

    -- Add use card mapping to current player
    local CurPlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId(self.OwnerState)
    GetGameService(self.OwnerState, GameServiceNameDef.GameInputService):AddInputMappingByPlayerId(CurPlayerId, UE.EInputMappingType.InputMapping_CardUsing)
end

function PlayerRoundState:OnExit()
    print("PlayerRoundState: OnExit")

    -- Stop listen to player card events
    GetGameService(self.OwnerState, GameServiceNameDef.LuaEventService):UnregisterListener(UE.ELuaEvent.LuaEvent_PlayerCardFinished, self, self.OnCardFinished)

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
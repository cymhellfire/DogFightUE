require "UnLua"

---@class PlayerRoundState Player can use cards in this state.
local PlayerRoundState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

function PlayerRoundState:OnEnter()
    print("PlayerRoundState: OnEnter")

    -- Add use card mapping to current player
    local CurPlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId()
    GetGameService(GameServiceNameDef.GameInputService):AddInputMappingByPlayerId(CurPlayerId, UE.EInputMappingType.InputMapping_CardUsing)

    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = "StandardMode.PostPlayerRoundCardState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
end

function PlayerRoundState:OnExit()
    print("PlayerRoundState: OnExit")

    -- Remove use card mapping from current player
    local CurPlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId()
    GetGameService(GameServiceNameDef.GameInputService):RemoveInputMappingByPlayerId(CurPlayerId, UE.EInputMappingType.InputMapping_CardUsing)
end

return PlayerRoundState
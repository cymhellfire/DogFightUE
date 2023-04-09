require "UnLua"

---@class PostPlayerRoundCardState : GameFlowStateLogicBase Discard cards that cannot be kept by current player.
local PostPlayerRoundCardState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

function PostPlayerRoundCardState:OnEnter()
    print("PostPlayerRoundCardState: OnEnter")

    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(self.OwnerState, GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = "StandardMode.PostPlayerRoundBuffState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
end

function PostPlayerRoundCardState:OnExit()
    print("PostPlayerRoundCardState: OnExit")
end

return PostPlayerRoundCardState
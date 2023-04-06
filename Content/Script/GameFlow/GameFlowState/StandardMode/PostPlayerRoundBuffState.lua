require "UnLua"

---@class PostPlayerRoundBuffState : GameFlowStateLogicBase Buff accumulation state after player round finished.
local PostPlayerRoundBuffState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

function PostPlayerRoundBuffState:OnEnter()
    print("PostPlayerRoundBuffState: OnEnter")

    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(self.OwnerState, GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = "StandardMode.GameEndCheckState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
end

function PostPlayerRoundBuffState:OnExit()
    print("PostPlayerRoundBuffState: OnExit")
end

return PostPlayerRoundBuffState
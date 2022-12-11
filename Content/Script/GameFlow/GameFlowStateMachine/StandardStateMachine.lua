require "UnLua"

local StandardStateMachine = Class()

function StandardStateMachine:K2_InitStateMachine()
    local InitStateArgument = GameServices.GameFlowStateService:GetGameFlowStateCreateArgument(self)
    InitStateArgument.StateName = "StandardMode.InitState"
    InitStateArgument.Instigator = self

    self:PushState(InitStateArgument)
end

return StandardStateMachine

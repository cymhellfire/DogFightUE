local StandardStateMachine = UnrealClass()

function StandardStateMachine:K2_InitStateMachine()
    local InitStateArgument = GetGameService(self, GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(self)
    InitStateArgument.StateName = "StandardMode.InitState"
    InitStateArgument.Instigator = self

    self:PushState(InitStateArgument)
end

return StandardStateMachine

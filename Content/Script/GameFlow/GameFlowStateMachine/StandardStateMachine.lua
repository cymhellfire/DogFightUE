local StandardStateMachine = UnrealClass()

local function InitStateMachineSettings(self)
    self.StateMachineSettings = {
        PlayerCount = 2,
    }
end

function StandardStateMachine:K2_InitStateMachine()
    InitStateMachineSettings(self)

    local InitStateArgument = GetGameService(self, GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(self)
    InitStateArgument.StateName = "StandardMode.InitState"
    InitStateArgument.Instigator = self

    self:PushState(InitStateArgument)
end

function StandardStateMachine:GetStateMachineSettings()
    return self.StateMachineSettings
end

return StandardStateMachine

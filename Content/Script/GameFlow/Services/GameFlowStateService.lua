local GameFlowStateService = UnrealClass("DogFight.Services.GameServiceBase")

function GameFlowStateService:StartupScript(ServiceName)
    self.Super.StartupScript(self, ServiceName)

    -- Setup variable
    self.DefaultGameFlowStateCreateArgumentPath = "GameFlow.GameFlowState.GameFlowStateCreateArgument"
    self.GameFlowStateMachineBasePath = "GameFlow.GameFlowStateMachine."
    self.GameFlowStateBasePath = "GameFlow.GameFlowState."
end

function GameFlowStateService:GetGameFlowStateCreateArgument(Instigator)
    local Class = UE.UGameFlowStateCreateArgument.StaticClass()
    local NewInstance = NewObject(Class, Instigator, nil, self.DefaultGameFlowStateCreateArgumentPath)
    return NewInstance
end

function GameFlowStateService:CreateStateMachine(InPath, Instigator)
    local Class = UE.UGameFlowStateMachine.StaticClass()
    local NewInstance = NewObject(Class, Instigator, nil, self.GameFlowStateMachineBasePath .. InPath)
    return NewInstance
end

return GameFlowStateService

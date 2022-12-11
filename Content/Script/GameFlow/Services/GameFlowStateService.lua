require "UnLua"

local GameFlowStateService = Class("DogFight.Services.GameServiceBase")

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

function GameFlowStateService:CreateGameFlowState(InArgument)
    if not InArgument then
        return
    end

    local Class = UE.UGameFlowStateBase.StaticClass()
    local NewInstance = NewObject(Class, InArgument.Instigator, nil)
    -- Set the create argument
    if NewInstance then
        NewInstance:SetCreateArgument(InArgument)
    end
    return NewInstance
end

return GameFlowStateService

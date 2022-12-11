require "UnLua"

local InitState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

local function DelayFinish(self, NextStateName)
    UE.UKismetSystemLibrary.Delay(self.OwnerState, 3)

    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GameServices.GameFlowStateService:GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = NextStateName
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
end

function InitState:OnEnter()
    print("InitState: OnEnter")
    print("InitState: Instigator " .. self.OwnerState.CreateArgument.Instigator:GetName())

    coroutine.resume(coroutine.create(DelayFinish), self, "StandardMode.SpawnState")
end

return InitState

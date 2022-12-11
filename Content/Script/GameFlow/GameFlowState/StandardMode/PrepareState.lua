require "UnLua"

local PrepareState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

local function DelayFinish(self, NextStateName)
    UE.UKismetSystemLibrary.Delay(self.OwnerState, 3)

    -- local Instigator = self.OwnerState.CreateArgument.Instigator
    -- local NewArgument = GameServices.GameFlowStateService:GetGameFlowStateCreateArgument(Instigator)
    -- if NewArgument then
    --     NewArgument.StateName = NextStateName
    --     NewArgument.Instigator = Instigator
    --     self.OwnerState:SetNextState(NewArgument)
    -- end

    self.OwnerState:Finish()
end

function PrepareState:OnEnter()
    print("PrepareState: OnEnter")

    coroutine.resume(coroutine.create(DelayFinish), self, "StandardMode.InitState")
end

function PrepareState:OnExit()
    print("PrepareState: OnExit")
end

return PrepareState
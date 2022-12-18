require "UnLua"
require "LuaIntegration.Common.UnrealUtils"

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

    --coroutine.resume(coroutine.create(DelayFinish), self, "StandardMode.SpawnState")

    local EventService = GameServices.LuaEventService
    if EventService then
        EventService:RegisterListener("LuaEvent_ReadyPlayerCount", self, self.OnReadyPlayerCountChanged)
    end
end

function InitState:OnReadyPlayerCountChanged(InCount)
    local GameInstance = GetGameInstance()
    local AllPlayerCount = GameInstance and GameInstance.GamePlayerCount or 0

    print("Ready Player: " .. InCount .. "/" .. AllPlayerCount)
end

return InitState

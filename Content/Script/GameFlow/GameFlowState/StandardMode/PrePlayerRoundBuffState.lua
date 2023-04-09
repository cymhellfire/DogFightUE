require "UnLua"

---@class PrePlayerRoundBuffState : GameFlowStateLogicBase Buff accumulation state before player round begin.
local PrePlayerRoundBuffState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

function PrePlayerRoundBuffState:OnEnter()
    local PlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId(self.OwnerState)
    print("PrePlayerRoundBuffState: OnEnter Player" .. PlayerId)

    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(self.OwnerState, GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = "StandardMode.PrePlayerRoundCardState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
end

function PrePlayerRoundBuffState:OnExit()
    print("PrePlayerRoundBuffState: OnExit")
end

return PrePlayerRoundBuffState
require "UnLua"

---@class GameEndCheckState Check if the game should end.
local GameEndCheckState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

function GameEndCheckState:OnEnter()
    print("GameEndCheckState: OnEnter")

    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GameServices.GameFlowStateService:GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = "StandardMode.GameSummaryState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
end

function GameEndCheckState:OnExit()
    print("GameEndCheckState: OnExit")
end

return GameEndCheckState
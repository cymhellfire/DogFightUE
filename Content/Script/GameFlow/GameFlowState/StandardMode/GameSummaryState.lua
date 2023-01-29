require "UnLua"

---@class GameSummaryState Show the game statistic.
local GameSummaryState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

function GameSummaryState:OnEnter()
    print("GameSummaryState: OnEnter")

    self.OwnerState:Finish()
end

function GameSummaryState:OnExit()
    print("GameSummaryState: OnExit")
end

return GameSummaryState
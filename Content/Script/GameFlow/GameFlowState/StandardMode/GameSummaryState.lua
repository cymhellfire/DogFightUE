require "UnLua"

---@class GameSummaryState Show the game statistic.
local GameSummaryState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

function GameSummaryState:OnEnter()
    print("GameSummaryState: OnEnter")

    self:GenerateGameSummary()
    self.OwnerState:Finish()
end

function GameSummaryState:OnExit()
    print("GameSummaryState: OnExit")
end

---Generate the game summary
function GameSummaryState:GenerateGameSummary()
    local AliveIdList = UE.UCommonGameplayFunctionLibrary.GetAlivePlayerId()
    local IdTable = AliveIdList:ToTable()
    if #IdTable > 0 then
        local WinnerId = ""
        for _, v in ipairs(IdTable) do
            WinnerId = WinnerId .. v
        end

        print("Winner is " .. WinnerId)
    else
        print("Draw")
    end
end

return GameSummaryState
require "UnLua"

---@class GameSummaryState : GameFlowStateLogicBase Show the game statistic.
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
    local AliveIdList = UE.UCommonGameplayFunctionLibrary.GetAlivePlayerId(self.OwnerState)
    local Result = ""
    local IdTable = AliveIdList:ToTable()
    if #IdTable > 0 then
        local WinnerId = ""
        for _, v in ipairs(IdTable) do
            WinnerId = WinnerId .. v
        end

        Result = "Winner is " .. WinnerId
    else
        Result = "Draw"
    end

    -- Send in-game chat message
    local MsgOption = UE.FInGameChatSendOption()
    MsgOption.Content = Result
    UE.UInGameMessageFunctionLibrary.SendInGameChatMessageAsSystem(self.OwnerState, MsgOption)
end

return GameSummaryState
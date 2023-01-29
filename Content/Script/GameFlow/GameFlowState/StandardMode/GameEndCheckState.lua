require "UnLua"

---@class GameEndCheckState Check if the game should end.
local GameEndCheckState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

function GameEndCheckState:OnEnter()
    print("GameEndCheckState: OnEnter")

    local bGameOver = self:CheckShouldGameEnd()
    if not bGameOver then
        -- Move the timeline forward
        UE.UCommonGameFlowFunctionLibrary.MoveTimelineForward()
        UE.UCommonGameFlowFunctionLibrary.SyncCurrentPlayerIdWithTimeline()
    end

    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = bGameOver and "StandardMode.GameSummaryState" or "StandardMode.PrePlayerRoundBuffState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
end

function GameEndCheckState:OnExit()
    print("GameEndCheckState: OnExit")
end

---Check if this game should end.
---@return boolean Should the game end?
function GameEndCheckState:CheckShouldGameEnd()
    return UE.UCommonGameplayFunctionLibrary.GetAlivePlayerNum() == 0
end

return GameEndCheckState
require "UnLua"

---@class GameEndCheckState : GameFlowStateLogicBase Check if the game should end.
local GameEndCheckState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

function GameEndCheckState:OnEnter()
    print("GameEndCheckState: OnEnter")

    local bGameOver = self:CheckShouldGameEnd()
    if not bGameOver then
        -- Move the timeline forward
        UE.UCommonGameFlowFunctionLibrary.MoveTimelineForward(self.OwnerState)
        UE.UCommonGameFlowFunctionLibrary.SyncCurrentPlayerIdWithTimeline(self.OwnerState)
    end

    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(self.OwnerState, GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
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
    return UE.UCommonGameplayFunctionLibrary.GetAlivePlayerNum(self.OwnerState) <= 1
end

return GameEndCheckState
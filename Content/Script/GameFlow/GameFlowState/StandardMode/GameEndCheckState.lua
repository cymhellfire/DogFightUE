require "UnLua"

---@class GameEndCheckState Check if the game should end.
local GameEndCheckState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

function GameEndCheckState:OnEnter()
    print("GameEndCheckState: OnEnter")

    -- Move the timeline forward
    UE.UCommonGameFlowFunctionLibrary.MoveTimelineForward()
    UE.UCommonGameFlowFunctionLibrary.SyncCurrentPlayerIdWithTimeline()

    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        --NewArgument.StateName = "StandardMode.GameSummaryState"
        NewArgument.StateName = "StandardMode.PrePlayerRoundBuffState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
end

function GameEndCheckState:OnExit()
    print("GameEndCheckState: OnExit")
end

return GameEndCheckState
require "UnLua"

---@class InitTimelineState State that initialize timeline for current game.
local InitTimelineState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

function InitTimelineState:OnEnter()
    print("InitTimelineState: OnEnter")

    UE.UCommonGameFlowFunctionLibrary.InitializeGameTimeline()
    self.OwnerState:Finish()
end

function InitTimelineState:OnExit()
    print("InitTimelineState: OnExit")
end

return InitTimelineState
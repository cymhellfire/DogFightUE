---@class InitTimelineState : GameFlowStateLogicBase State that initialize timeline for current game.
local InitTimelineState = UnrealClass("GameFlow.GameFlowState.GameFlowStateLogicBase")

function InitTimelineState:OnEnter()
    print("InitTimelineState: OnEnter")

    -- Initialize timeline
    UE.UCommonGameFlowFunctionLibrary.InitializeGameTimeline(self.OwnerState)
    -- Sync the current player id
    UE.UCommonGameFlowFunctionLibrary.SyncCurrentPlayerIdWithTimeline(self.OwnerState)

    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(self.OwnerState, GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = "StandardMode.PrePlayerRoundBuffState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end
    
    self.OwnerState:Finish()
end

function InitTimelineState:OnExit()
    print("InitTimelineState: OnExit")
end

return InitTimelineState
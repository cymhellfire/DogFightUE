---@class GameEndCheckState : GameFlowStateLogicBase Check if the game should end.
local GameEndCheckState = UnrealClass("GameFlow.GameFlowState.GameFlowStateLogicBase")

local EntityStateMap = {
    [UE.EGameTimelineEntityType.Player] = "StandardMode.PrePlayerRoundBuffState",
}

function GameEndCheckState:OnEnter()
    print("GameEndCheckState: OnEnter")

    local StartRoundStateName
    local StartEntityId = UE.UCommonGameFlowFunctionLibrary.GetCurrentTimelineEntityId(self.OwnerState)
    local bGameOver = self:CheckShouldGameEnd()
    if not bGameOver then
        -- Move the timeline forward until find any valid state name
        while not StartRoundStateName or #StartRoundStateName == 0 do
            UE.UCommonGameFlowFunctionLibrary.MoveTimelineForward(self.OwnerState)
            -- Break the loop if we iterate through all the timeline
            local CurEntityId = UE.UCommonGameFlowFunctionLibrary.GetCurrentTimelineEntityId(self.OwnerState)
            local CurEntityType = UE.UCommonGameFlowFunctionLibrary.GetCurrentTimelineEntityType(self.OwnerState)
            StartRoundStateName = EntityStateMap[CurEntityType]
            if CurEntityId == StartEntityId then
                -- Also mark as game over
                -- bGameOver = true
                break
            end
        end
        if StartRoundStateName and #StartRoundStateName > 0 then
            UE.UCommonGameFlowFunctionLibrary.SyncCurrentPlayerIdWithTimeline(self.OwnerState)
        end
    end

    -- Throw error message when state machine is block
    if not bGameOver and not StartRoundStateName then
        error("GameEndCheckState:OnEnter No valid next state to continue.")
        return
    end

    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(self.OwnerState, GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = bGameOver and "StandardMode.GameSummaryState" or StartRoundStateName --"StandardMode.PrePlayerRoundBuffState"
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
require "UnLua"

---@class PrePlayerRoundCardState Dispatch cards to current player in this state.
local PrePlayerRoundCardState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

function PrePlayerRoundCardState:OnEnter()
    print("PrePlayerRoundCardState: OnEnter")

    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GameServices.GameFlowStateService:GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = "StandardMode.PlayerRoundState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
end

function PrePlayerRoundCardState:OnExit()
    print("PrePlayerRoundCardState: OnExit")
end

return PrePlayerRoundCardState
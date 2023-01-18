require "UnLua"

---@class PlayerRoundState Player can use cards in this state.
local PlayerRoundState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

function PlayerRoundState:OnEnter()
    print("PlayerRoundState: OnEnter")

    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GameServices.GameFlowStateService:GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = "StandardMode.PostPlayerRoundCardState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
end

function PlayerRoundState:OnExit()
    print("PlayerRoundState: OnExit")
end

return PlayerRoundState
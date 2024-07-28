---@field OwnerState GameFlowState
---@class GameFlowStateLogicBase
local GameFlowStateLogicBase = UnrealClass()

---Initialize logic with owner state.
---@param InState GameFlowState Owner game flow state.
function GameFlowStateLogicBase:Init(InState)
    self.OwnerState = InState
end

function GameFlowStateLogicBase:GetStateMachineSettings()
    return self.OwnerState.CreateArgument.Instigator and self.OwnerState.CreateArgument.Instigator:GetStateMachineSettings()
end

return GameFlowStateLogicBase

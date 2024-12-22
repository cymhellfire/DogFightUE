---@field OwnerState GameFlowState
---@class GameFlowStateLogicBase
local GameFlowStateLogicBase = UnrealClass()

---Initialize logic with owner state.
---@param InState GameFlowState Owner game flow state.
---@param ExtraData table Extra data of this state.
function GameFlowStateLogicBase:Init(InState, ExtraData)
    self.OwnerState = InState
    self.ExtraData = ExtraData
end

function GameFlowStateLogicBase:GetStateMachineSettings()
    return self.OwnerState.CreateArgument.Instigator and self.OwnerState.CreateArgument.Instigator:GetStateMachineSettings()
end

function GameFlowStateLogicBase:GetExtraData()
    return self.ExtraData
end

return GameFlowStateLogicBase

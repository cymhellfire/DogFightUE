require "UnLua"

local GameFlowStateLogicBase = Class()

function GameFlowStateLogicBase:Init(InState)
    self.OwnerState = InState
end

return GameFlowStateLogicBase

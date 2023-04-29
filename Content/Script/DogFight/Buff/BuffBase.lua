---@class BuffBase
---@field _Logic BuffLogicBase Buff logic script class.
local BuffBase = UnrealClass()

function BuffBase:LoadBuffScript(InPath)
    local LogicTemp = require(InPath)
    if LogicTemp then
        self._Logic = LogicTemp:New()
        self._Logic:Init(self)
    end
end

---@param InCharacter ATopDownStylePlayerCharacter
function BuffBase:ApplyScript(InCharacter)
    if self._Logic then
        self._Logic:OnApply(InCharacter)
    end
end

---@param InCharacter ATopDownStylePlayerCharacter
function BuffBase:RemoveScript(InCharacter)
    if self._Logic then
        self._Logic:OnRemove(InCharacter)
    end
end

return BuffBase
---@class BuffLogicBase
---@field _Owner BuffBase Owner buff instance.
local BuffLogicBase = UnrealClass()

---@param InBuff BuffBase Owner buff instance.
function BuffLogicBase:Init(InBuff)
    self._Owner = InBuff
end

---@param InCharacter ATopDownStylePlayerCharacter
function BuffLogicBase:OnApply(InCharacter)
    print("[BuffLogic] " .. self:tostring() .. " applied.")
end

---@param InCharacter ATopDownStylePlayerCharacter
function BuffLogicBase:OnRemove(InCharacter)
    print("[BuffLogic] " .. self:tostring() .. " removed.")
end

function BuffLogicBase:tostring()
    return "BuffLogicBase"
end

return BuffLogicBase
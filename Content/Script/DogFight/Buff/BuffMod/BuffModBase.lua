---@class BuffModBase Modification of buff with extra functionality.
---@field _BuffLogic BuffLogicBase Owner buff logic instance.
---@field _Buff BuffBase Owner buff object instance.
local BuffModBase = UnrealClass()

---@param InLogic BuffLogicBase
function BuffModBase:OnInit(InLogic)
    print("[BuffModBase] OnInit: " .. self:tostring())
    self._BuffLogic = InLogic
    self._Buff = InLogic._Owner
end

function BuffModBase:OnUninit()
    print("[BuffModBase] OnUninit: " .. self:tostring())
end

function BuffModBase:tostring()
    return "BuffModBase"
end

return BuffModBase
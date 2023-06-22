---@class AttributeModifierScriptBase Base class of all modifier scripts.
---@field _Owner AttributeModifierBase Owner modifier instance.
local AttributeModifierScriptBase = UnrealClass()

---Initialize modifier script.
---@param InModifierBase AttributeModifierBase Owner modifier base instance.
---@param Config table Modifier config to initialize with.
function AttributeModifierScriptBase:Init(InModifierBase, Config)
    self._Owner = InModifierBase

    --- Initialize the modifier here
    if type(self.InitModifier) == "function" then
        self:InitModifier(Config)
    end
end

---Initialize description object for this modifier script.
---@param InDescObject UAttributeModifierDescObject Description object to initialize.
function AttributeModifierScriptBase:InitDescObject(InDescObject)

end

return AttributeModifierScriptBase
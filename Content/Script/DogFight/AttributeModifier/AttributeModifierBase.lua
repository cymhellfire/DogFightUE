---@class AttributeModifierBase : ULuaAttributeModifierObject Base class that hold modifier scripts.
---@field _ModifierScirpt AttributeModifierScriptBase Modifier script instance.
local AttributeModifierBase = UnrealClass()

---Load and initialize modifier script.
---@param InScript string Path of script to load.
function AttributeModifierBase:LoadAndInitModifierScript(InScript)
    local ScriptTemp = require(InScript)
    if ScriptTemp then
        self._ModifierScirpt = ScriptTemp:New()
        -- Initialize new script instance
        self._ModifierScirpt:Init(self)
    else
        print("[AttributeModifierBase] Failed to load modifier script: " .. InScript)
    end
end

---@param InDescObject UAttributeModifierDescObject
function AttributeModifierBase:InitDescObjectScript(InDescObject)
    if self._ModifierScirpt then
        -- Let the script do initialize things
        self._ModifierScirpt:InitDescObject(InDescObject)
    end
end

return AttributeModifierBase
---@class AttributeModifierBase : ULuaAttributeModifierObject Base class that hold modifier scripts.
---@field _ModifierScirpt AttributeModifierScriptBase Modifier script instance.
local AttributeModifierBase = UnrealClass()

---Load and initialize modifier script.
---@param InId number Id of modifier config.
function AttributeModifierBase:LoadAndInitModifierScript(InId)
    local Config
    if self.ModifierType == UE.ELuaAttributeModifierType.AMT_CardAttribute then
        ---@type CardModifierService
        local CardModifierService = GetGameService(self, GameServiceNameDef.CardModifierService)
        if CardModifierService then
            Config = CardModifierService.Config:GetConfig(InId)
        end
    elseif self.ModifierType == UE.ELuaAttributeModifierType.AMT_CharacterStatus then
        ---@type CharacterModifierService
        local CharacterModifierService = GetGameService(self, GameServiceNameDef.CharacterModifierService)
        if CharacterModifierService then
            Config = CharacterModifierService.Config:GetConfig(InId)
        end
    end

    if Config == nil or Config.Script == nil then
        return
    end

    local ScriptTemp = require(Config.Script)
    if ScriptTemp then
        self._ModifierScirpt = ScriptTemp:New()
        -- Initialize new script instance
        self._ModifierScirpt:Init(self, Config)
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

---Initialize this modifier with given arguments
---@param InArgs table Arguments to initialize with.
function AttributeModifierBase:SetupArguments(InArgs)
    if self._ModifierScirpt then
        self._ModifierScirpt:SetupArguments(InArgs)
    end
end

return AttributeModifierBase
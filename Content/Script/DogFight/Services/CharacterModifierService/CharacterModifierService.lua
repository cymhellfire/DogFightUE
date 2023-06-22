---@class CharacterModifierService : GameServiceBase, UCharacterModifierService Service provide character modifier spawning and management.
local CharacterModifierService = UnrealClass("DogFight.Services.GameServiceBase")

function CharacterModifierService:GetConfigPath()
    return "DogFight.Services.CharacterModifierService.CharacterModifierConfig"
end

---Spawn character modifier with given id.
---@param InId number Id of character modifier to spawn.
---@return UCharacterStatusModifier New modifier instance.
function CharacterModifierService:SpawnCharacterModifierInternal(InId)
    local Config = self.Config:GetConfig(InId)
    if not Config then
        return
    end

    ---@type UCharacterStatusModifier
    local NewModifierObject = NewObject(UE.UCharacterStatusModifier.StaticClass(), self)

    ---Load the logic script
    if NewModifierObject then
        NewModifierObject.ModifierType = UE.ELuaAttributeModifierType.AMT_CharacterStatus
        NewModifierObject:LoadAndInitModifierScript(InId)
    end

    return NewModifierObject
end

return CharacterModifierService
local CharacterModifierTypeDef = require "DogFight.Services.CharacterModifierService.CharacterModifierTypeDef"

---@class CharacterModifierConfig : ServiceConfigBase Config data of all character status modifiers.
local CharacterModifierConfig = UnrealClass("DogFight.Services.ServiceConfigBase")

local Config = {
    [CharacterModifierTypeDef.Add10PhysicalResistance] = {
        Name = "Add 10 Physical Resistance",
        Desc = "Add 10 Physical Resistance",
        Script = "DogFight.AttributeModifier.CharacterStatusModifier.CSM_Add10PhysResist",
    },
}

function CharacterModifierConfig:GetData()
    return Config
end

return CharacterModifierConfig
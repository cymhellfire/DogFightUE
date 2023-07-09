local CharacterAttributePresetTypeDef = require "DogFight.Services.CharacterAttributePresetService.CharacterAttributePresetTypeDef"
local AttributeNameDef = require "DogFight.Common.AttributeNameDef"

---@class CharacterAttributePresetConfig : ServiceConfigBase
local CharacterAttributePresetConfig = UnrealClass("DogFight.Services.ServiceConfigBase")

local Config = {
    [CharacterAttributePresetTypeDef.Default] = {
        {Name = AttributeNameDef.HandCardCapacity, DataType = UE.EAttributeDataType.ADT_Integer, InitValue = 2},
    }
}

function CharacterAttributePresetConfig:GetData()
    return Config
end

return CharacterAttributePresetConfig
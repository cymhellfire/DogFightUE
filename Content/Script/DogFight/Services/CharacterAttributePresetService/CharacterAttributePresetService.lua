require "Common.StringHelper"

---@class CharacterAttributePresetService : GameServiceBase, UCharacterAttributePresetService
local CharacterAttributePresetService = UnrealClass("DogFight.Services.GameServiceBase")

---@param InData table
---@return FAttributeCreateArgument
local function ConvertToAttributeCreateArgument(InData)
    if type(InData.Name) == "string" and #InData.Name > 0 then
        local NewArgument = UE.FAttributeCreateArgument()
        NewArgument.AttrName = InData.Name
        NewArgument.DataType = InData.DataType
        if NewArgument.DataType == UE.EAttributeDataType.ADT_Boolean then
            NewArgument.InitBooleanValue = InData.InitValue
        elseif NewArgument.DataType == UE.EAttributeDataType.ADT_Integer then
            NewArgument.InitIntegerValue = InData.InitValue
        elseif NewArgument.DataType == UE.EAttributeDataType.ADT_Float then
            NewArgument.InitFloatValue = InData.InitValue
        end

        return NewArgument
    end
end

---@param InComponent UGameplayAttributesComponent Attribute holder component to initialize.
---@param ConfigId number Id of config to initialize with.
function CharacterAttributePresetService:InitAttributePreset(InComponent, ConfigId)
    -- Skip if component is invalid
    if InComponent == nil then
        return
    end

    -- Get config
    local Config = self.Config:GetConfig(ConfigId)
    if not Config then
        return
    end

    -- Add attributes based on config
    for _, v in ipairs(Config) do
        local NewArgument = ConvertToAttributeCreateArgument(v)
        if NewArgument then
            InComponent:AddAttribute(NewArgument)
        end
    end
end

function CharacterAttributePresetService:GetConfigPath()
    return "DogFight.Services.CharacterAttributePresetService.CharacterAttributePresetConfig"
end

return CharacterAttributePresetService

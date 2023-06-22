---@class CAM_CommonModifier : AttributeModifierScriptBase Common card attribute modifier script.
local CAM_CommonModifier = UnrealClass("DogFight.AttributeModifier.AttributeModifierScriptBase")

---@param Config table 
function CAM_CommonModifier:InitModifier(Config)
    if Config.Data then
        local DataType = Config.Data.DataType or UE.EAttributeDataType.ADT_Integer
        --- Create modifier based config
        local ModifierArugment = UE.FModifierCreateArgument()
        ModifierArugment.DataType = DataType
        ModifierArugment.OperatorType = Config.Data.OperatorType or UE.EModifierOperatorType.MOT_Addition
        if DataType == UE.EAttributeDataType.ADT_Integer then
            ModifierArugment.InitIntegerValue = Config.Data.InitValue or 0
        elseif DataType == UE.EAttributeDataType.ADT_Float then
            ModifierArugment.InitFloatValue = Config.Data.InitValue or 0
        elseif DataType == UE.EAttributeDataType.ADT_Boolean then
            ModifierArugment.InitBooleanValue = Config.Data.InitValue or false
        end

        self._Owner:CreateModifier(ModifierArugment)
    end

    -- Record display info
    self._Info = Config.Info
end

---@param InDescObject UAttributeModifierDescObject
function CAM_CommonModifier:InitDescObject(InDescObject)
    InDescObject:SetSourceString(self._Owner:GetName())
    InDescObject:SetEffectString(self._Owner:GetEffectString())

    local AttributeWrapper = self._Owner:GetModifiedAttributeObject()
    if AttributeWrapper then
        local AttrName = AttributeWrapper:GetDisplayName()
        local EffectString = self._Owner:GetEffectString()
        local LocalizedDesc = GetLocalizedString(LocalizationTable.CardDisplay, "Mod_SimpleDesc", AttrName, EffectString)
        InDescObject:SetDescString(LocalizedDesc)
    end
end

return CAM_CommonModifier
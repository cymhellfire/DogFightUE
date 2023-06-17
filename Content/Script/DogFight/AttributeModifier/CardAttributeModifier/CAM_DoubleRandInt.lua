---@class CAM_DoubleRandInt : AttributeModifierScriptBase Double the value of a random integer.
local CAM_DoubleRandInt = UnrealClass("DogFight.AttributeModifier.AttributeModifierScriptBase")

function CAM_DoubleRandInt:InitModifier()
    local ModifierArugment = UE.FModifierCreateArgument()
    ModifierArugment.DataType = UE.EAttributeDataType.ADT_Integer
    ModifierArugment.OperatorType = UE.EModifierOperatorType.MOT_Multiply
    ModifierArugment.InitIntegerValue = 2

    self._Owner:CreateModifier(ModifierArugment)
end

---@param InDescObject UAttributeModifierDescObject
function CAM_DoubleRandInt:InitDescObject(InDescObject)
    InDescObject:SetSourceString(self._Owner:GetName())
    InDescObject:SetEffectString("x2")

    local AttributeWrapper = self._Owner:GetModifiedAttributeObject()
    if AttributeWrapper then
        InDescObject:SetDescString(AttributeWrapper:GetAttributeName() .. " X2")
    end
end

return CAM_DoubleRandInt
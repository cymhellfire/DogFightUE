local AttributeNameDef = require "DogFight.Common.AttributeNameDef"

---@class CSM_Add10PhysResist : AttributeModifierScriptBase Add 10 physcial resistance to character.
local CSM_Add10PhysResist = UnrealClass("DogFight.AttributeModifier.AttributeModifierScriptBase")

function CSM_Add10PhysResist:InitModifier(Config)
    local ModifierArugment = UE.FModifierCreateArgument()
    ModifierArugment.DataType = UE.EAttributeDataType.ADT_Integer
    ModifierArugment.OperatorType = UE.EModifierOperatorType.MOT_Addition
    ModifierArugment.InitIntegerValue = 10
    ModifierArugment.ApplyRuleString = "Name=PhysResist"

    self._Owner:CreateModifier(ModifierArugment)
end

function CSM_Add10PhysResist:SetupArguments(InArgs)
    if type(InArgs) ~= "table" then
        return
    end

    local ResistValue = InArgs[AttributeNameDef.ResistValue] or 0
    self._Owner:SetModifierIntegerFactor(ResistValue)
end

return CSM_Add10PhysResist
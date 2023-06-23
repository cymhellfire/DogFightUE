local CardModifierTypeDef = require "DogFight.Services.CardModifierService.CardModifierTypeDef"

---@class CardModifierConfig : ServiceConfigBase
local CardModifierConfig = UnrealClass("DogFight.Services.ServiceConfigBase")

local Config = {
    [CardModifierTypeDef.DoubleRandomInt] = {
        Script = "DogFight.AttributeModifier.CardAttributeModifier.CAM_CommonModifier",
        Data = {
            DataType = UE.EAttributeDataType.ADT_Integer,
            OperatorType = UE.EModifierOperatorType.MOT_Multiply,
            InitValue = 2,
        },
    }
}

function CardModifierConfig:GetData()
    return Config
end

return CardModifierConfig
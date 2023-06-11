local CardModifierTypeDef = require "DogFight.Services.CardModifierService.CardModifierTypeDef"

---@class CardModifierConfig : ServiceConfigBase
local CardModifierConfig = UnrealClass("DogFight.Services.ServiceConfigBase")

local Config = {
    [CardModifierTypeDef.DoubleRandomInt] = {
        Script = "DogFight.AttributeModifier.CardAttributeModifier.CAM_DoubleRandInt",
    }
}

function CardModifierConfig:GetData()
    return Config
end

return CardModifierConfig
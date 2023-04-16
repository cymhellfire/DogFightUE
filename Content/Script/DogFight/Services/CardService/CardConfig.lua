local CardTypeDef = require("DogFight.Services.CardService.CardTypeDef")

---@class CardConfig : ServiceConfigBase
local CardConfig = Class("DogFight.Services.ServiceConfigBase")

local Config = {
    [CardTypeDef.CharacterMove] = {
        Name = "CharacterMove",
        LogicPath = "DogFight.Card.CardLogic.LogicCharacterMove",
    },
    [CardTypeDef.Attack] = {
        Name = "Attack",
        LogicPath = "DogFight.Card.CardLogic.LogicFireball",
    },
}

function CardConfig:GetData()
    return Config
end

return CardConfig
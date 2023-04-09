local CardNameDef = require("DogFight.Services.CardService.CardNameDef")

---@class CardConfig : ServiceConfigBase
local CardConfig = Class("DogFight.Services.ServiceConfigBase")

local Config = {
    [CardNameDef.CharacterMove] = {
        Id = 0,
        LogicPath = "DogFight.Card.CardLogic.LogicCharacterMove",
    },
    [CardNameDef.Attack] = {
        Id = 1,
        LogicPath = "DogFight.Card.CardLogic.TestCardLogic",
    }
}

function CardConfig:GetData()
    return Config
end

return CardConfig
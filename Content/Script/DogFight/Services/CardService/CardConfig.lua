local CardTypeDef = require("DogFight.Services.CardService.CardTypeDef")

---@class CardConfig : ServiceConfigBase
local CardConfig = UnrealClass("DogFight.Services.ServiceConfigBase")

local Config = {
    [CardTypeDef.CharacterMove] = {
        Name = "CharacterMove",
        LogicPath = "DogFight.Card.CardLogic.LogicCharacterMove",
    },
    [CardTypeDef.Attack] = {
        Name = "Attack",
        LogicPath = "DogFight.Card.CardLogic.LogicFireball",
    },
    [CardTypeDef.AddPhysResist] = {
        Name = "AddPhysResist",
        LogicPath = "DogFight.Card.CardLogic.LogicAddPhysResist",
    },
}

function CardConfig:GetData()
    return Config
end

return CardConfig
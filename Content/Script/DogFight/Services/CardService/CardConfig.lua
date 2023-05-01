local CardTypeDef = require("DogFight.Services.CardService.CardTypeDef")
local BuffTypeDef = require "DogFight.Services.BuffService.BuffTypeDef"

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
        LogicPath = "DogFight.Card.CardLogic.LogicAddBuff",
        LogicParam = {
            BuffInfo = {
                BuffId = BuffTypeDef.AddPhysResist,
                Duration = 1,
            },
        },
    },
    [CardTypeDef.LuckyBomb] = {
        Name = "LuckyBomb",
        LogicPath = "DogFight.Card.CardLogic.LogicAddBuff",
        LogicParam = {
            BuffInfo = {
                BuffId = BuffTypeDef.LuckyBomb,
            },
        },
    },
}

function CardConfig:GetData()
    return Config
end

return CardConfig
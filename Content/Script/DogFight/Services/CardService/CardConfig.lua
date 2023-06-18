local CardTypeDef = require("DogFight.Services.CardService.CardTypeDef")
local BuffTypeDef = require "DogFight.Services.BuffService.BuffTypeDef"

---@class CardConfig : ServiceConfigBase
local CardConfig = UnrealClass("DogFight.Services.ServiceConfigBase")

local Config = {
    [CardTypeDef.CharacterMove] = {
        Name = "CharacterMove",
        LogicPath = "DogFight.Card.CardLogic.LogicCharacterMove",
        LogicParam = {
            CardInfo = {
                Name = "Name_CharacterMove",
                Desc = {
                    Key = "Desc_CharacterMove",
                }
            }
        }
    },
    [CardTypeDef.Attack] = {
        Name = "Attack",
        LogicPath = "DogFight.Card.CardLogic.LogicFireball",
        LogicParam = {
            CardInfo = {
                Desc = {
                    Key = "Desc_Fireball",
                    Param = {
                        "[Int]Damage",
                    },
                }
            },
            AttrInfo = {
                {Name = "Damage", DisplayName = "Attr_Damage", Type = "Integer", Value = 10},
            },
        },
    },
    [CardTypeDef.AddPhysResist] = {
        Name = "AddPhysResist",
        LogicPath = "DogFight.Card.CardLogic.LogicAddBuff",
        LogicParam = {
            CardInfo = {
                Name = "PhysShield",
            },
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
            CardInfo = {
                Name = "LuckyBomb",
            },
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
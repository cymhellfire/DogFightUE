local CardTypeDef = require("DogFight.Services.CardService.CardTypeDef")
local BuffTypeDef = require "DogFight.Services.BuffService.BuffTypeDef"
local AttributeNameDef = require "DogFight.Common.AttributeNameDef"

local AttributeEnum = require ("DogFight.DataBridge.AttributeEnum")

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
                Name = "Name_Fireball",
                Desc = {
                    Key = "Desc_Fireball",
                    Param = {
                        "[Int]" .. AttributeNameDef.Damage,
                    },
                }
            },
            AttrInfo = {
                {Name = AttributeNameDef.Damage, Type = AttributeEnum.DataType.Integer, Value = 10},
                {Name = AttributeNameDef.ProjectileSpeed, Type = AttributeEnum.DataType.Integer, Value = 500},
            },
        },
    },
    [CardTypeDef.AddPhysResist] = {
        Name = "AddPhysResist",
        LogicPath = "DogFight.Card.CardLogic.LogicAddBuff",
        LogicParam = {
            CardInfo = {
                Name = "Name_PhysResist",
                Desc = {
                    Key = "Desc_PhysResist",
                    Param = {
                        "[Int]" .. AttributeNameDef.ResistValue,
                    },
                }
            },
            BuffInfo = { 
                BuffId = BuffTypeDef.AddPhysResist,
                Duration = 1,
                Arguments = {
                    ResistValue = "[Int]" .. AttributeNameDef.ResistValue,
                }
            },
            AttrInfo = {
                {Name = AttributeNameDef.ResistValue, Type = AttributeEnum.DataType.Integer, Value = 10},
            }
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
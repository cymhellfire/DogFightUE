local CardTypeDef = require("DogFight.Services.CardService.CardTypeDef")
local BuffTypeDef = require "DogFight.Services.BuffService.BuffTypeDef"
local ProjectileTypeDef = require "DogFight.Services.ProjectileService.ProjectileTypeDef"
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
            },
            TargetInfo = {
                Count = 1,
                Type = UE.ECardTargetType.CTT_Point,
            },
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
            TargetInfo = {
                Count = 1,
                Type = UE.ECardTargetType.CTT_Actor,
            },
            ProjectileInfo = {
                ProjectileId = ProjectileTypeDef.Fireball,
                Damage = "[Int]" .. AttributeNameDef.Damage,
                Speed = "[Int]" .. AttributeNameDef.ProjectileSpeed,
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
            TargetInfo = {
                Count = 1,
                Type = UE.ECardTargetType.CTT_Actor,
            },
            BuffInfo = { 
                BuffId = BuffTypeDef.AddPhysResist,
                Duration = 1,
                ResistValue = "[Int]" .. AttributeNameDef.ResistValue,
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
                Name = "Name_LuckyBomb",
                Desc = {
                    Key = "Desc_LuckyBomb",
                    Param = {
                        "[Float][Percent]" .. AttributeNameDef.BuffRatio,
                        "[Int]" .. AttributeNameDef.Damage,
                        "[Float]" .. AttributeNameDef.DamageRadius,
                    }
                }
            },
            TargetInfo = {
                Count = 1,
                Type = UE.ECardTargetType.CTT_Actor,
            },
            BuffInfo = {
                BuffId = BuffTypeDef.LuckyBomb,
                BuffRatio = "[Float]" .. AttributeNameDef.BuffRatio,
                Damage = "[Int]" .. AttributeNameDef.Damage,
                DamageRadius = "[Float]" .. AttributeNameDef.DamageRadius,
            },
            AttrInfo = {
                {Name = AttributeNameDef.Damage, Type = AttributeEnum.DataType.Integer, Value = 10},
                {Name = AttributeNameDef.DamageRadius, Type = AttributeEnum.DataType.Float, Value = 250},
                {Name = AttributeNameDef.BuffRatio, Type = AttributeEnum.DataType.Float, Value = 0.35},
            }
        },
    },
    [CardTypeDef.HolyShield] = {
        Name = "HolyShield",
        LogicPath = "DogFight.Card.CardLogic.LogicAddBuff",
        LogicParam = {
            CardInfo = {
                Name = "Name_HolyShield",
                Desc = {
                    Key = "Desc_HolyShield",
                    Param = {
                        "[Int]" .. AttributeNameDef.BuffDuration,
                    }
                }
            },
            TargetInfo = {
                Count = 1,
                Type = UE.ECardTargetType.CTT_Actor,
            },
            BuffInfo = {
                BuffId = BuffTypeDef.Invincible,
                Duration = "[Int]" .. AttributeNameDef.BuffDuration,
            },
            AttrInfo = {
                {Name = AttributeNameDef.BuffDuration, Type = AttributeEnum.DataType.Integer, Value = 1},
            }
        }
    },
    [CardTypeDef.NuclearStrike] = {
        Name = "NuclearStrike",
        LogicPath = "DogFight.Card.CardLogic.LogicRandomDropProjectile",
        LogicParam = {
            CardInfo = {
                Name = "Name_NuclearStrike",
                Desc = {
                    Key = "Desc_NuclearStrike",
                    Param = {
                        "[Int]" .. AttributeNameDef.Damage,
                        "[Float]" .. AttributeNameDef.DamageRadius,
                    }
                }
            },
            TargetInfo = {
                Count = "[Int]" .. AttributeNameDef.TargetCount,
                Type = UE.ECardTargetType.CTT_Point,
                Random = true,
            },
            ProjectileInfo = {
                ProjectileId = ProjectileTypeDef.NuclearBomb,
                SpawnHeight = 1000,
                Damage = "[Int]" .. AttributeNameDef.Damage,
                DamageRadius = "[Float]" .. AttributeNameDef.DamageRadius,
            },
            AttrInfo = {
                {Name = AttributeNameDef.Damage, Type = AttributeEnum.DataType.Integer, Value = 50},
                {Name = AttributeNameDef.DamageRadius, Type = AttributeEnum.DataType.Float, Value = 500},
                {Name = AttributeNameDef.TargetCount, Type = AttributeEnum.DataType.Integer, Value = 1},
            }
        }
    },
}

function CardConfig:GetData()
    return Config
end

return CardConfig
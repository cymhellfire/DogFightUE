local AvatarNameDef = require("DogFight.Services.AvatarManagerService.AvatarNameDef")

---@class AvatarConfig : ServiceConfigBase
local AvatarConfig = UnrealClass("DogFight.Services.ServiceConfigBase")

local Config = {
    [AvatarNameDef.Greystone] = {
        Asset = "/Script/ActionGame.AvatarDataAsset'/Game/DogFightGame/DataAsset/Avatar/AD_Greystone.AD_Greystone'"
    },
    [AvatarNameDef.Shinbi] = {
        Asset = "/Script/ActionGame.AvatarDataAsset'/Game/DogFightGame/DataAsset/Avatar/AD_Shinbi.AD_Shinbi'"
    }
}

function AvatarConfig:GetData()
    return Config
end

return AvatarConfig

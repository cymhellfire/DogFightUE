---@class AvatarManagerService : GameServiceBase, UAvatarManagerService Service to manage all avatars data.
local AvatarManagerService = UnrealClass("DogFight.Services.GameServiceBase")

function AvatarManagerService:GetConfigPath()
    return "DogFight.Services.AvatarManagerService.AvatarConfig"
end

---Get avatar config by id.
---@param ConfigId number Id of config to get.
function AvatarManagerService:GetAvatarConfig(ConfigId)
    local Config = self.Config:GetConfig(ConfigId)
    return Config
end

return AvatarManagerService
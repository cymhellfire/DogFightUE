---@class AvatarManagerService : GameServiceBase, UAvatarManagerService Service to manage all avatars data.
local AvatarManagerService = UnrealClass("DogFight.Services.GameServiceBase")

function AvatarManagerService:GetAvatarDescAssetPathByConfigId(ConfigId)
    local Config = self:GetAvatarConfig(ConfigId)
    return Config and Config.Asset
end

function AvatarManagerService:GetConfigPath()
    return "DogFight.Services.AvatarManagerService.AvatarConfig"
end

---Get avatar config by id.
---@param ConfigId number Id of config to get.
function AvatarManagerService:GetAvatarConfig(ConfigId)
    local Config = self.Config:GetConfig(ConfigId)
    return Config
end

local AvatarGroupIndexDef = {
    AllRandom = 1,
}

local AvatarGroupDef = {
    [AvatarGroupIndexDef.AllRandom] = {
        Name = "Char_Random",
    },
}

---Get option list of avatar selection widget.
function AvatarManagerService:GetAvatarSelectOptionList()
    local OptionList = {}
    -- Add group first
    for _, v in ipairs(AvatarGroupDef) do
        OptionList[#OptionList + 1] = v.Name
    end
    -- Add all avatar
    for _, v in ipairs(self.Config.Data) do
        OptionList[#OptionList + 1] = v.Name
    end

    return OptionList
end

---@param InId FGameLobbyPlayerAvatarId
function AvatarManagerService:GetOptionIndexByAvatarId(InId)
    local Index = 0
    if InId.Type == UE.EGameLobbyPlayerAvatarIdType.EPAIT_AvatarId then
        Index = InId.Id + #AvatarGroupDef
    elseif InId.Type == UE.EGameLobbyPlayerAvatarIdType.EPAIT_GroupId then
        Index = InId.Id
    end
    return Index - 1
end

---Convert avatar selection widget index to lobby player avatar id
---@param InIndex number Widget index
---@return FGameLobbyPlayerAvatarId
function AvatarManagerService:GetLobbyPlayerAvatarId(InIndex)
    local NewId = UE.FGameLobbyPlayerAvatarId()

    if InIndex <= #AvatarGroupDef then
        NewId.Type = UE.EGameLobbyPlayerAvatarIdType.EPAIT_GroupId
        NewId.Id = InIndex
    else
        NewId.Type = UE.EGameLobbyPlayerAvatarIdType.EPAIT_AvatarId
        NewId.Id = InIndex - #AvatarGroupDef
    end

    return NewId
end

---@param InId FGameLobbyPlayerAvatarId
function AvatarManagerService:GetOptionLocalizationString(InId)
    if InId.Type == UE.EGameLobbyPlayerAvatarIdType.EPAIT_AvatarId then
        local Config = self:GetAvatarConfig(InId.Id)
        if Config then
            return GetLocalizedString(LocalizationTable.Character, Config.Name)
        end
    elseif InId.Type == UE.EGameLobbyPlayerAvatarIdType.EPAIT_GroupId then
        local Config = AvatarGroupDef[InId.Id]
        if Config then
            return GetLocalizedString(LocalizationTable.Character, Config.Name)
        end
    end
    return "invalid"
end

return AvatarManagerService
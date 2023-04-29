require "Common.StringHelper"

---@field Config ServiceConfigBase Table that contains all config data.
---@class GameServiceBase Base class of all game services.
local GameServiceBase = UnrealClass()

function GameServiceBase:StartupScript(ServiceName)
    print("ServiceStart: " .. ServiceName)

    -- Load config if exists
    local ConfigPath = nil
    if self.GetConfigPath then
        ConfigPath = self:GetConfigPath()
        if ConfigPath then
            self:LoadConfig(ConfigPath)
        end
    end

    -- Register this service name to global scope
    if _G.GameServiceNameDef == nil then
        ---@type table Contains names of all game services alive.
        _G.GameServiceNameDef = {}
    end

    -- Name duplication check
    if _G.GameServiceNameDef[ServiceName] ~= nil then
        print("[GameServiceBase] Duplicated service name detected: " .. ServiceName)
        return
    end

    _G.GameServiceNameDef[ServiceName] = ServiceName
end

function GameServiceBase:ShutdownScript(ServiceName)
    print("ServiceStop: " .. ServiceName)

    -- Unregister this service from global scope
    if _G.GameServiceNameDef ~= nil and _G.GameServiceNameDef[ServiceName] ~= nil then
        _G.GameServiceNameDef[ServiceName] = nil
    end
end

---Load config data from given path.
---@param InPath string Path of config data file.
function GameServiceBase:LoadConfig(InPath)
    local ConfigClass = require(InPath)
    ---@type ServiceConfigBase
    if ConfigClass then
        self.Config = ConfigClass:New()
    end
end

---Get the config file path to load.
---@return string Path of config file.
function GameServiceBase:GetConfigPath()
end

---Validate script path with specified base path.
function GameServiceBase:ValidateScriptPath(InPath)
    if string.startWith(InPath, self:GetScriptBasePath()) then
        return InPath
    end

    return self:GetScriptBasePath() .. InPath
end

function GameServiceBase:GetScriptBasePath()
    return "DogFight."
end

return GameServiceBase

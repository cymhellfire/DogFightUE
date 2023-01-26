require "UnLua"
require "Common.StringHelper"

local GameServiceBase = Class()

function GameServiceBase:StartupScript(ServiceName)
    print("ServiceStart: " .. ServiceName)

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

require "UnLua"

local UGameServiceBase = Class()

function UGameServiceBase:StartupScript(ServiceName)
    print("ServiceStart: " .. ServiceName)

    -- Register this service to global scope
    if _G.GameServices == nil then
        _G.GameServices = {}
    end

    -- Name duplication check
    if _G.GameServices[ServiceName] ~= nil then
        print("[GameServiceBase] Duplicated service name detected: " .. ServiceName)
        return
    end

    _G.GameServices[ServiceName] = self
end

function UGameServiceBase:ShutdownScript(ServiceName)
    print("ServiceStop: " .. ServiceName)

    -- Unregister this service from global scope
    if _G.GameServices ~= nil and _G.GameServices[ServiceName] == self then
        _G.GameServices[ServiceName] = nil
    end
end

return UGameServiceBase

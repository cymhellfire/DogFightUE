---@class LuaGameInstanceSubsystem : ULuaGameInstanceSubsystem
local LuaGameInstanceSubsystem = UnrealClass()

function LuaGameInstanceSubsystem:ScriptInit()
    ---@type LuaGameInstance
    local LuaGameInstance = self:GetLuaGameInstance()
    if LuaGameInstance then
        LuaGameInstance:RegisterSubsystem(self)
    end
end

function LuaGameInstanceSubsystem:ScriptDeinit()
    ---@type LuaGameInstance
    local LuaGameInstance = self:GetLuaGameInstance()
    if LuaGameInstance then
        LuaGameInstance:UnregisterSubsystem(self)
    end
end

return LuaGameInstanceSubsystem
require("LuaPanda").start("127.0.0.1",8818)
require "UnrealClass"
require "DogFight.Services.Utils.ServiceUtils"
require "LuaIntegration.Common.UnrealUtils"

---@class LuaGameInstance : ULuaGameInstance
---@field SubsystemMap table Table contains all subsystem instances.
local LuaGameInstance = UnrealClass()

---@param InSubsystem LuaGameInstanceSubsystem
function LuaGameInstance:RegisterSubsystem(InSubsystem)
    if not InSubsystem then
        return
    end

    if self.SubsystemMap == nil then
        self.SubsystemMap = {}
    end

    local SubsystemName = InSubsystem:GetSubsystemName()
    self.SubsystemMap[SubsystemName] = InSubsystem

    -- Register subsystem name
    if not _G.GameSubsystemNameDef then
        _G.GameSubsystemNameDef = {}
    end

    -- Duplicate check
    if _G.GameSubsystemNameDef[SubsystemName] then
        error(string:format("Dulipcate subsystem detected %s"), SubsystemName)
        return
    end

    _G.GameSubsystemNameDef[SubsystemName] = SubsystemName
end

---@param InSubsystem LuaGameInstanceSubsystem
function LuaGameInstance:UnregisterSubsystem(InSubsystem)
    if not InSubsystem then
        return
    end

    local SubsystemName = InSubsystem:GetSubsystemName()
    if self.SubsystemMap.SubsystemName ~= nil then
        self.SubsystemMap.SubsystemName = nil
    end
end

---@param InName string Name of subsystem to get.
---@return ULuaGameInstanceSubsystem
function LuaGameInstance:GetSubsystem(InName)
    if self.SubsystemMap then
        return self.SubsystemMap[InName]
    end
end

return LuaGameInstance
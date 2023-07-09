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

    self.SubsystemMap[InSubsystem:GetSubsystemName()] = InSubsystem
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
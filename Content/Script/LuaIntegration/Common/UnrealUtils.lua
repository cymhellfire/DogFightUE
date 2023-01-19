require "UnLua"

--- Wrapper functions for common Unreal Engine interfaces

---Get current world.
---@return UWorld Current world.
local function GetCurrentWorld()
    return UE.ULuaIntegrationFunctionLibrary.GetCurrentWorld()
end

---Get game instance.
---@return UGameInstance Current game instance.
local function GetGameInstance()
    return UE.ULuaIntegrationFunctionLibrary.GetGameInstance()
end

---Load a class by path.
---@param InPath string Resource path of class.
---@return UClass Loaded class.
local function LoadClass(InPath)
    return UE.ULuaIntegrationFunctionLibrary.LoadClassByPath(InPath)
end

---Check whether a object is derived from specified class.
---@param InObject UObject Object to check with.
---@param InClass UClass Class to check.
---@return boolean Is the object derived from class.
local function IsDerivedFrom(InObject, InClass)
    return UE.ULuaIntegrationFunctionLibrary.IsDerivedFrom(InObject, InClass)
end

_G.GetCurrentWorld = GetCurrentWorld
_G.GetGameInstance = GetGameInstance
_G.LoadClass = LoadClass
_G.IsDerivedFrom = IsDerivedFrom
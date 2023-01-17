require "UnLua"

--- Wrapper functions for common Unreal Engine interfaces

local function GetCurrentWorld()
    return UE.ULuaIntegrationFunctionLibrary.GetCurrentWorld()
end

local function GetGameInstance()
    return UE.ULuaIntegrationFunctionLibrary.GetGameInstance()
end

local function LoadClass(InPath, InOuter)
    return UE.ULuaIntegrationFunctionLibrary.LoadClassByPath(InPath, InOuter)
end

_G.GetCurrentWorld = GetCurrentWorld
_G.GetGameInstance = GetGameInstance
_G.LoadClass = LoadClass
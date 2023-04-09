require "UnLua"
local ArrayHelper = require("Common.ArrayHelper")

--- Wrapper functions for common Unreal Engine interfaces

---Get current world.
---@param WorldContextObject UObject Object instance to get world with.
---@return UWorld Current world.
local function GetCurrentWorld(WorldContextObject)
    return UE.ULuaIntegrationFunctionLibrary.GetCurrentWorld(WorldContextObject)
end

---Get game instance.
---@param WorldContextObject UObject Object instance to get world with.
---@return UGameInstance Current game instance.
local function GetGameInstance(WorldContextObject)
    return UE.ULuaIntegrationFunctionLibrary.GetGameInstance(WorldContextObject)
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

---Get the first local player controller.
---@param WorldContextObject UObject Object instance to get world with.
---@return APlayerController Player controller.
local function GetLocalPlayerController(WorldContextObject)
    return UE.ULuaIntegrationFunctionLibrary.GetFirstLocalPlayerController(WorldContextObject)
end

---Get localized version string in current culture.
---@param WorldContextObject UObject Object instance to get world with.
---@param InTable string Name of string table to search localization.
---@param InKey string Key of the localization data.
---@param ParamList table 
---@return string Localized version string.
local function GetLocalizedString(WorldContextObject, InTable, InKey, ParamList)
    if type(ParamList) == "table" then
        return UE.ULuaIntegrationFunctionLibrary.GetLocalizedStringWithParam(WorldContextObject, InTable, InKey, ArrayHelper.StringArrayFromTable(ParamList))
    else
        return UE.ULuaIntegrationFunctionLibrary.GetLocalizedString(WorldContextObject, InTable, InKey)
    end
end

_G.GetCurrentWorld = GetCurrentWorld
_G.GetGameInstance = GetGameInstance
_G.LoadClass = LoadClass
_G.IsDerivedFrom = IsDerivedFrom
_G.GetLocalPlayerController = GetLocalPlayerController
_G.GetLocalizedString = GetLocalizedString
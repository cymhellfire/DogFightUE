require "UnLua"
local ArrayHelper = require("Common.ArrayHelper")

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

---Get the first local player controller.
---@return APlayerController Player controller.
local function GetLocalPlayerController()
    return UE.ULuaIntegrationFunctionLibrary.GetFirstLocalPlayerController()
end

---Get localized version string in current culture.
---@param InTable string Name of string table to search localization.
---@param InKey string Key of the localization data.
---@param ParamList table 
---@return string Localized version string.
local function GetLocalizedString(InTable, InKey, ParamList)
    if type(ParamList) == "table" then
        return UE.ULuaIntegrationFunctionLibrary.GetLocalizedStringWithParam(InTable, InKey, ArrayHelper.StringArrayFromTable(ParamList))
    else
        return UE.ULuaIntegrationFunctionLibrary.GetLocalizedString(InTable, InKey)
    end
end

_G.GetCurrentWorld = GetCurrentWorld
_G.GetGameInstance = GetGameInstance
_G.LoadClass = LoadClass
_G.IsDerivedFrom = IsDerivedFrom
_G.GetLocalPlayerController = GetLocalPlayerController
_G.GetLocalizedString = GetLocalizedString
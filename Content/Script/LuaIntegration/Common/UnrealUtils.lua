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

---Enum of all localization strint table.
local LocalizationTable = {
    CardDisplay = "ST_CardDisplay",
    Attribute = "ST_Attribute",
}

---Generic notify receiver function for lua side.
---@param Service LuaEventService Lua event service instance.
---@param NotifyIndex number Lua event index.
local function ReceiveNotifyFromC(Service, NotifyIndex, ...)
    if Service then
        Service:OnReceiveNativeNotify(NotifyIndex, ...)
    end
end

---Get localized string.
---@param InTable string Name of localization string table.
---@param InKey string Localization key.
---@param ... any Arguments of localization string.
local function GetLocalizedString(InTable, InKey, ...)
    return _G.GetLocalizedString(InTable, InKey, ...)
end

_G.GetCurrentWorld = GetCurrentWorld
_G.GetGameInstance = GetGameInstance
_G.LoadClass = LoadClass
_G.IsDerivedFrom = IsDerivedFrom
_G.GetLocalPlayerController = GetLocalPlayerController
_G.LocalizationTable = LocalizationTable
_G.ReceiveNotifyFromC = ReceiveNotifyFromC
_G.GetLocalizedString = GetLocalizedString

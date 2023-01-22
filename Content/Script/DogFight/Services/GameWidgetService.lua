require "UnLua"
require "LuaIntegration.Common.UnrealUtils"

local WidgetNameDef = require("DogFight.Services.Config.GameWidgetNameDef")
local WidgetConfig = require("DogFight.Services.Config.GameWidgetConfig")

---@class GameWidgetService Service that handle all widget relative operations.
local GameWidgetService = Class("DogFight.Services.GameServiceBase")

function GameWidgetService:StartupScript(ServiceName)
    self.Super.StartupScript(self, ServiceName)

    ---@type table Map that contains all currently displaying widgets.
    self.AliveWidgetMap = {}
end

---Show widget with specified name.
---@param InWidget string Name of widget to display.
---@return boolean Whether the operation succeed.
function GameWidgetService:ShowWidget(InWidget)
    -- Skip if the widget is already shown
    if self:IsWidgetShown(InWidget) then
        return true
    end

    local WidgetCfg = WidgetConfig[InWidget]
    if WidgetCfg then
        -- Load the resource
        local LoadedClass = UE.ULuaIntegrationFunctionLibrary.LoadClassByPath(WidgetCfg.Path)
        if not LoadedClass then
            print("[GameWidgetService] Failed to load resource: " .. WidgetCfg.Path)
            return false
        end

        ---@type UUserWidget
        local NewWidget = UE.UWidgetBlueprintLibrary.Create(GetCurrentWorld(), LoadedClass, GetLocalPlayerController())
        if NewWidget then
            NewWidget:AddToViewport()

            -- Record it to alive widget map
            self.AliveWidgetMap[InWidget] = NewWidget
        end
    end
end

---Hide widget with specified name.
---@param InWidget string Name of widget to hide.
---@return boolean Whether the operation succeed.
function GameWidgetService:HideWidget(InWidget)
    -- Skip if the widget is not displaying
    if not self:IsWidgetShown(InWidget) then
        return true
    end

    ---@type UUserWidget
    local OpWidget = self.AliveWidgetMap[InWidget]
    if OpWidget then
        OpWidget:RemoveFromParent()

        -- Remove the record
        self.AliveWidgetMap[InWidget] = nil
    end
end

---Check if given widget is currently displaying.
---@param InWidget string Name of widget to check.
---@return boolean Whether the widget is displaying.
function GameWidgetService:IsWidgetShown(InWidget)
    -- Check in alive widget map
    return self.AliveWidgetMap[InWidget] or false
end



return GameWidgetService
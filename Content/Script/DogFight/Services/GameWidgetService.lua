require "LuaIntegration.Common.UnrealUtils"

local WidgetNameDef = require("DogFight.Services.Config.GameWidgetNameDef")
local WidgetConfig = require("DogFight.Services.Config.GameWidgetConfig")

---@class GameWidgetService : UGameWidgetService Service that handle all widget relative operations.
local GameWidgetService = UnrealClass("DogFight.Services.GameServiceBase")

local function LoadAndCreateWidget(self, InWidget, bShow)
    local WidgetCfg = WidgetConfig[InWidget]
    if WidgetCfg then
        -- Load the resource
        local LoadedClass = UE.ULuaIntegrationFunctionLibrary.LoadClassByPath(WidgetCfg.Path)
        if not LoadedClass then
            print("[GameWidgetService] Failed to load resource: " .. WidgetCfg.Path)
            return false
        end

        ---@type UUserWidget
        local NewWidget = UE.UWidgetBlueprintLibrary.Create(GetCurrentWorld(self), LoadedClass, GetLocalPlayerController(self))
        if NewWidget then
            NewWidget:AddToViewport()
            -- Use different behavior based on bShow flag
            if bShow then
                -- Record it to shown widget map
                self.ShownWidgetMap[InWidget] = NewWidget
            else
                NewWidget:SetVisibility(UE.ESlateVisibility.Collapsed)
                -- Record it to hidden map
                self.HiddenWidgetMap[InWidget] = NewWidget
            end
            return true
        end
    end

    return false
end

local function RemoveAndDestroyWidget(self, InWidget)
    ---@type UUserWidget
    local OpWidget = self.HiddenWidgetMap[InWidget]
    if OpWidget then
        -- Invoke destruct
        OpWidget:Destruct()

        OpWidget:RemoveFromParent()

        -- Remove the record
        self.HiddenWidgetMap[InWidget] = nil
        return true
    end

    return false
end

function GameWidgetService:StartupScript(ServiceName)
    self.Super.StartupScript(self, ServiceName)

    ---@type table Map that contains all currently displaying widgets.
    self.ShownWidgetMap = {}
    ---@type table Map that contains all hidden widgets.
    self.HiddenWidgetMap = {}
end

function GameWidgetService:PostStartupScript()
    -- Listen to ShowWidget event
    ---@type LuaEventService
    local LuaEventService = GetGameService(self, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_ShowWidget, self, self.OnShowWidgetEvent)
    end
end

function GameWidgetService:OnShowWidgetEvent(InWidget, ...)
    self:ShowWidget(InWidget, true, table.pack(...))
end

---Create and add a new widget to viewport.
---@param InWidget string Name of widget to create.
---@param bSilent boolean Not display new created widget after creation.
---@return boolean Whether the operation succeed.
function GameWidgetService:AddWidget(InWidget, bSilent)
    -- Skip if the widget is already shown
    if self:IsWidgetShown(InWidget) then
        return true
    end

    -- Check if this widget is created
    if self:IsWidgetCreated(InWidget) then
        if bSilent then
            return true
        end
        return self:ShowWidget(InWidget)
    end

    -- Create specified widget now
    return LoadAndCreateWidget(self, InWidget, not bSilent)
end

---Hide and remove a widget from viewport.
---@param InWidget string Name of widget to remove.
---@return boolean Whether the operation succeed.
function GameWidgetService:RemoveWidget(InWidget)
    -- Skip if the widget is not created
    if not self:IsWidgetCreated(InWidget) then
        return false
    end

    -- Hide the widget first
    if self:IsWidgetShown(InWidget) then
        self:HideWidget(InWidget)
    end

    -- Remove the widget
    return RemoveAndDestroyWidget(self, InWidget)
end

---Show widget with specified name.
---@param InWidget string Name of widget to display.
---@param bCreate boolean Create widget if no existing instance.
---@param Params any Packed params that pass into shown widget.
---@return UUserWidget Widget instance that shown.
function GameWidgetService:ShowWidget(InWidget, bCreate, Params)
    -- Skip if the widget is already shown
    if self:IsWidgetShown(InWidget) then
        return self:GetWidget(InWidget)
    end

    if not self:IsWidgetCreated(InWidget) then
        if not bCreate then
            return nil
        end
        -- Create a new instance here
        LoadAndCreateWidget(self, InWidget)
    end

    -- Show the widget if it exists
    ---@type UUserWidget
    local Widget = self.HiddenWidgetMap[InWidget]
    if Widget then
        Widget:SetVisibility(UE.ESlateVisibility.SelfHitTestInvisible)
        -- Trigger OnShow function
        if type(Widget.OnShow) == "function" then
            if type(Params) == "table" then
                Widget:OnShow(table.unpack(Params))
            else
                Widget:OnShow(Params)
            end
        end
        return Widget
    end

    return nil
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
    local OpWidget = self.ShownWidgetMap[InWidget]
    if OpWidget then
        OpWidget:SetVisibility(UE.ESlateVisibility.Collapsed)

        -- Remove the record and add to hidden map
        self.ShownWidgetMap[InWidget] = nil
        self.HiddenWidgetMap[InWidget] = OpWidget
        return true
    end

    return false
end

---Check if given widget is currently displaying.
---@param InWidget string Name of widget to check.
---@return boolean Whether the widget is displaying.
function GameWidgetService:IsWidgetShown(InWidget)
    -- Check in alive widget map
    return self.ShownWidgetMap[InWidget] or false
end

---Check if given widget is already created.
---@param InWidget string Name of widget to check.
---@return boolean Whether the widget is exist.
function GameWidgetService:IsWidgetCreated(InWidget)
    return self.ShownWidgetMap[InWidget] or self.HiddenWidgetMap[InWidget] or false
end

---Get created widget by name.
---@param InWidget string Name of widget to get.
---@return UUserWidget The widget with given name.
function GameWidgetService:GetWidget(InWidget)
    local Result = self.ShownWidgetMap[InWidget]
    if Result == nil then
        Result = self.HiddenWidgetMap[InWidget]
    end

    return Result
end

return GameWidgetService
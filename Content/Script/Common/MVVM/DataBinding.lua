---@class DataBinding Constructor class for all data binding.
local DataBinding = UnrealClass()

---Create data binding to directly modify the text.
function DataBinding.TextContextBinding()
    local NewBinding = {}
    NewBinding.SetValue = function(self, widget, value)
        if type(widget.SetText) == "function" then
            widget:SetText(value)
        else
            error(widget:GetName() .. " does NOT have function named SetText()")
        end
    end
    return NewBinding
end

---Create data binding to modify the color and opacity of widget.
function DataBinding.ColorAndOpacityBinding()
    local NewBinding = {}
    NewBinding.SetValue = function(self, widget, value)
        if type(widget.SetColorAndOpacity) == "function" then
            widget:SetColorAndOpacity(value)
        else
            error(widget:GetName() .. " does NOT have function named SetColorAndOpacity()")
        end
    end
    return NewBinding
end

---Create data binding to modify the active widget index of switcher.
function DataBinding.SwitcherIndexBinding()
    local NewBinding = {}
    ---@param widget UWidgetSwitcher
    ---@param value number
    NewBinding.SetValue = function(self, widget, value)
        if type(value) ~= "number" then
            return
        end
        if type(widget.SetActiveWidgetIndex) == "function" then
            widget:SetActiveWidgetIndex(value)
        else
            error(widget:GetName() .. " does NOT have function named SetActiveWidgetIndex()")
        end
    end
    return NewBinding
end

---@param bMatchSize boolean
function DataBinding.TexturePathBinding(bMatchSize)
    local NewBinding = {}
    NewBinding.bMatchSize = bMatchSize
    ---@param widget UImage
    ---@param value string
    NewBinding.SetValue = function(self, widget, value)
        if type(value) ~= "string" then
            return
        end

        UE.UCommonWidgetFunctionLibrary.SetImageByTexturePath(widget, value, self.bMatchSize or false)
    end
    return NewBinding
end

---@param bMatchSize boolean
function DataBinding.TextureAssetBinding(bMatchSize)
    local NewBinding = {}
    NewBinding.bMatchSize = bMatchSize
    ---@param widget UImage
    ---@param value UTexture2D
    NewBinding.SetValue = function(self, widget, value)
        if not value then
            return
        end

        UE.UCommonWidgetFunctionLibrary.SetImageByTextureAsset(widget, value, self.bMatchSize or false)
    end
    return NewBinding
end

---Create data binding to modify the widget enable state.
function DataBinding.WidgetEnableBinding()
    local NewBinding = {}
    ---@param widget UWidget
    ---@param value boolean
    NewBinding.SetValue = function(self, widget, value)
        if type(value) ~= "boolean" then
            error("WidgetEnableBinding need boolean type value input.")
            return
        end

        widget:SetIsEnabled(value)
    end
    return NewBinding
end

---Create data binding that control visibility of widget.
---@param HitTest boolean Whether this widget is hitable when show.
function DataBinding.WidgetVisibilityBinding(HitTest)
    local NewBinding = {}
    NewBinding.bHitTest = HitTest
    ---@param widget UWidget
    ---@param value boolean
    NewBinding.SetValue = function(self, widget, value)
        if type(value) ~= "boolean" then
            error("WidgetVisibilityBinding need boolean type value input.")
            return
        end

        local Visibility
        if value then
            if self.bHitTest then
                Visibility = UE.ESlateVisibility.Visible
            else
                Visibility = UE.ESlateVisibility.SelfHitTestInvisible
            end
        else
            Visibility = UE.ESlateVisibility.Collapsed
        end
        widget:SetVisibility(Visibility)
    end
    return NewBinding
end

return DataBinding
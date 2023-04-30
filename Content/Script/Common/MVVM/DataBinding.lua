---@class DataBinding Constructor class for all data binding.
local DataBinding = UnrealClass()

---Create data binding to directly modify the text.
function DataBinding.TextContextBinding()
    local NewBinding = {}
    NewBinding.SetValue = function(self, widget, value)
        widget:SetText(value)
    end
    return NewBinding
end

---Create data binding to modify the color and opacity of widget.
function DataBinding.ColorAndOpacityBinding()
    local NewBinding = {}
    NewBinding.SetValue = function(self, widget, value)
        widget:SetColorAndOpacity(value)
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
        widget:SetActiveWidgetIndex(value)
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

return DataBinding
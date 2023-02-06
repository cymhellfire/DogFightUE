require "UnLua"

---@class DataBinding Constructor class for all data binding.
local DataBinding = Class()

---Create data binding to directly modify the text.
function DataBinding.TextContextBinding()
    local NewBinding = {}
    NewBinding.SetValue = function(widget, value)
        widget:SetText(value)
    end
    return NewBinding
end

---Create data binding to modify the color and opacity of widget.
function DataBinding.ColorAndOpacityBinding()
    local NewBinding = {}
    NewBinding.SetValue = function(widget, value)
        widget:SetColorAndOpacity(value)
    end
    return NewBinding
end

---Create data binding to modify the active widget index of switcher.
function DataBinding.SwitcherIndexBinding()
    local NewBinding = {}
    NewBinding.SetValue = function(widget, value)
        if type(value) ~= "number" then
            return
        end
        widget:SetActiveWidgetIndex(value)
    end
    return NewBinding
end

return DataBinding
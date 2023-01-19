require "UnLua"

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

return DataBinding
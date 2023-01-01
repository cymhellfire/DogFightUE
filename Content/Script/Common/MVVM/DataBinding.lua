require "UnLua"

local DataBinding = Class()

function DataBinding.TextContextBinding()
    local NewBinding = {}
    NewBinding.SetValue = function(widget, value)
        widget:SetText(value)
    end
    return NewBinding
end

return DataBinding
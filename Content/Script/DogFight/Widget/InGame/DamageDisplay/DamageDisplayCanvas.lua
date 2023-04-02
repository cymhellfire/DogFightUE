require "UnLua"

---@class DamageDisplayCanvas : BP_Widget_DamageDisplayCanvas_C
local DamageDisplayCanvas = Class("Common.MVVM.ModelBase")
local DamageDisplayCanvasVM = require("DogFight.Widget.InGame.DamageDisplay.DamageDisplayCanvasVM")
local ViewModelBase = require("Common.MVVM.ViewModelBase")

function DamageDisplayCanvas:Initialize()

end

---Add new damage display unit to this canvas.
---@param InUnit DamageDisplayUnit Display unit instance.
function DamageDisplayCanvas:AddUnit(InUnit)
    if not InUnit then
        return
    end

    self.DisplayCanvas:AddChild(InUnit)
end

return DamageDisplayCanvas
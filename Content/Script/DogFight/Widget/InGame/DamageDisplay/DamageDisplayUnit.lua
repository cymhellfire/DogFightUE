require "UnLua"

---@class DamageDisplayUnit : BP_Widget_DamageDisplayUnit_C
local DamageDisplayUnit = Class("Common.MVVM.ModelBase")
local DamageDisplayUnitVM = require("DogFight.Widget.InGame.DamageDisplay.DamageDisplayUnitVM")
local WidgetNameDef = require("DogFight.Services.Config.GameWidgetNameDef")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")

function DamageDisplayUnit:Initialize()
    local NewVM = InstantiateViewModel(DamageDisplayUnitVM)
    self:BindViewModel(NewVM, {
        {BindKey = "DamageValueText",   UIKey = "DamageText",   DataBinding = DataBinding.TextContextBinding(), }
    })
end

function DamageDisplayUnit:PostInitialized()
    
end

---Triggered when take damage.
---@param DamageInstance UExtendedDamageInstance Damage instance has taken.
---@param Value number Damage value has taken.
function DamageDisplayUnit:AddDamageItem(DamageInstance, Value)
    self.ViewModel.DamageValueText = Value
end

function DamageDisplayUnit:OnOwnerActivated()
    -- Add myself to damage display canvas
    ---@type GameWidgetService
    local GameWidgetService = GetGameService(GameServiceNameDef.GameWidgetService)
    if GameWidgetService then
        ---@type DamageDisplayCanvas
        local Canvas = GameWidgetService:GetWidget(WidgetNameDef.WidgetDamageDisplay)
        -- Open canvas automatically
        if not Canvas then
            Canvas = GameWidgetService:ShowWidget(WidgetNameDef.WidgetDamageDisplay, true)
        end
        if Canvas then
            Canvas:AddUnit(self)

            -- Play animation
            self:PlayAnimation(self.Active)
        end
    end
end

function DamageDisplayUnit:OnOwnerDead()
    -- Hide from screen
    self:RemoveFromParent()
end

return DamageDisplayUnit
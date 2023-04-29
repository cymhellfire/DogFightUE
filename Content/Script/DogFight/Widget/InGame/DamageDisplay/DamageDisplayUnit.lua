---@class DamageDisplayUnit : BP_Widget_DamageDisplayUnit_C
local DamageDisplayUnit = UnrealClass("Common.MVVM.ModelBase")
local DamageDisplayUnitVM = require("DogFight.Widget.InGame.DamageDisplay.DamageDisplayUnitVM")
local WidgetNameDef = require("DogFight.Services.Config.GameWidgetNameDef")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")

function DamageDisplayUnit:Initialize()
    local NewVM = InstantiateViewModel(DamageDisplayUnitVM)
    self:BindViewModel(NewVM, {
        {BindKey = "DamageValueText",   UIKey = "DamageText",   DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "DamageColor",       UIKey = "DamageText",   DataBinding = DataBinding.ColorAndOpacityBinding() },
    })
end

function DamageDisplayUnit:PostInitialized()
    
end

---Triggered when take damage.
---@param DamageInstance UExtendedDamageInstance Damage instance has taken.
---@param DisplayParam FDamageDisplayParams Damage display parameter set.
function DamageDisplayUnit:AddDamageItem(DisplayParam)
    -- Get display parameter from damage config
    local DamageStyle
    ---@type DamageService
    local DamageService = GetGameService(self, GameServiceNameDef.DamageService)
    if DamageService then
        local DamageConfig = DamageService.Config:GetConfig(DisplayParam.DamageId)
        if DamageConfig then
            DamageStyle = DamageConfig.DisplayParams
        end
    end

    self.ViewModel.DamageValueText = DisplayParam.DamageValue
    if DamageStyle then
        -- Set the damage color
        local Color = DamageStyle.Color
        if Color then
            self.ViewModel.DamageColor = UE.UCommonWidgetFunctionLibrary.MakeSlateColor(Color[1], Color[2], Color[3], 1)
        end
    end
end

function DamageDisplayUnit:OnOwnerActivated()
    -- Add myself to damage display canvas
    ---@type GameWidgetService
    local GameWidgetService = GetGameService(self, GameServiceNameDef.GameWidgetService)
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
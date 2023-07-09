---@class PlayerCharacterStateUnitView : ModelBase 
local PlayerCharacterStateUnitView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local PlayerCharacterStateUnitVM = require("DogFight.Widget.InGame.PlayerCharacterState.PlayerCharacterStateUnitVM")
--local ListWrapper = require("Common.ListView.ListViewWrapper")
local WidgetNameDef = require("DogFight.Services.GameWidgetService.GameWidgetNameDef")

function PlayerCharacterStateUnitView:PostInitialized()
    local NewVM = InstantiateViewModel(PlayerCharacterStateUnitVM)
    self:BindViewModel(NewVM, {
        {BindKey = "HealthText",   UIKey = "Health_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })

    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)
end

function PlayerCharacterStateUnitView:OnActive()
    -- Add myself to player character state canvas
    ---@type GameWidgetService
    local GameWidgetService = GetGameService(self, GameServiceNameDef.GameWidgetService)
    if GameWidgetService then
        ---@type PlayerCharacterStateCanvasView
        local Canvas = GameWidgetService:GetWidget(WidgetNameDef.WidgetCharacterState)
        -- Open canvas automatically
        if not Canvas then
            Canvas = GameWidgetService:ShowWidget(WidgetNameDef.WidgetCharacterState, true)
        end
        if Canvas then
            Canvas:AddStateUnit(self)
        end
    end
end

---Triggered when character health changed.
---@param CurHealth number Current health value.
---@param MaxHealth number Maximum helath value.
function PlayerCharacterStateUnitView:OnHealthChanged(CurHealth, MaxHealth)
    self.ViewModel.HealthText = tostring(CurHealth) .. "/" .. tostring(MaxHealth)
end

return PlayerCharacterStateUnitView
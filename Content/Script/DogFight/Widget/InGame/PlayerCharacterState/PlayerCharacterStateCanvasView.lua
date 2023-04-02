---@class PlayerCharacterStateCanvasView : ModelBase 
local PlayerCharacterStateCanvasView = Class("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local PlayerCharacterStateCanvasVM = require("DogFight.Widget.InGame.PlayerCharacterState.PlayerCharacterStateCanvasVM")
--local ListWrapper = require("Common.ListView.ListViewWrapper")

function PlayerCharacterStateCanvasView:Initialize()
    local NewVM = InstantiateViewModel(PlayerCharacterStateCanvasVM)
    self:BindViewModel(NewVM, {
        --{BindKey = "TitleMessage",   UIKey = "TitleMessage_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })
end

function PlayerCharacterStateCanvasView:PostInitialized()
    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)
end

---Add a new player state unit to canvas.
---@param InUnit PlayerCharacterStateUnitView
function PlayerCharacterStateCanvasView:AddStateUnit(InUnit)
    if not InUnit then
        return
    end

    self.DisplayCanvas:AddChild(InUnit)
end

return PlayerCharacterStateCanvasView
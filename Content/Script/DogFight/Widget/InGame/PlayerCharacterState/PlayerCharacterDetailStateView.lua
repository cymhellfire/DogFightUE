---@class PlayerCharacterDetailStateView : ModelBase 
local PlayerCharacterDetailStateView = Class("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local PlayerCharacterDetailStateVM = require("DogFight.Widget.InGame.PlayerCharacterState.PlayerCharacterDetailStateVM")
--local ListWrapper = require("Common.ListView.ListViewWrapper")

function PlayerCharacterDetailStateView:Initialize()
    local NewVM = InstantiateViewModel(PlayerCharacterDetailStateVM)
    self:BindViewModel(NewVM, {
        {BindKey = "CharacterName",   UIKey = "CharacterName_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })
end

function PlayerCharacterDetailStateView:PostInitialized()
    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)
end

---@param InCharacter ACharacter
---@param MousePos FVector2D
function PlayerCharacterDetailStateView:OnShow(InCharacter, MousePos)
    if InCharacter then
        self.ViewModel.CharacterName = InCharacter:GetName()
    end

    -- Sync mouse position
    self:SyncPosition(MousePos)
end

---@param InPos FVector2D
function PlayerCharacterDetailStateView:SyncPosition(InPos)
    local ViewportScale = UE.UWidgetLayoutLibrary.GetViewportScale(self)

    UE.UWidgetLayoutLibrary.SlotAsCanvasSlot(self.SyncPositionPanel):SetPosition(UE.FVector2D(InPos.X / ViewportScale, InPos.Y / ViewportScale))
end

return PlayerCharacterDetailStateView
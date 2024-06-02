---@class PreparationRoomView : ModelBase
local PreparationRoomView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local PreparationRoomVM = require("DogFight.Widget.PreparationRoom.PreparationRoomVM")
--local ListWrapper = require("Common.ListView.ListViewWrapper")

local GameLuaStateNameDef = require("DogFight.Services.GameStateMachineService.GameLuaStateNameDef")

function PreparationRoomView:PostInitialized()
    local NewVM = InstantiateViewModel(PreparationRoomVM)
    self:BindViewModel(NewVM, {
        {BindKey = "CharacterName",   UIKey = "CharacterName_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })

    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)

    self.CharacterSelect_ComboBox.OnSelectionChanged:Add(self, self.OnCharacterSelectChanged)
    self.BackButton.OnClicked:Add(self, self.OnBackButtonClicked)
end

function PreparationRoomView:UnInitialize()
    self.CharacterSelect_ComboBox.OnSelectionChanged:Remove(self, self.OnCharacterSelectChanged)
    self.BackButton.OnClicked:Remove(self, self.OnBackButtonClicked)
end

function PreparationRoomView:OnShow()
    
end

function PreparationRoomView:OnCharacterSelectChanged(Item, SelectionType)
    print("PreparationRoomView:OnCharacterSelectChanged ", Item)

    self.ViewModel.CharacterName = GetLocalizedString(LocalizationTable.Character, Item)
end

function PreparationRoomView:OnBackButtonClicked()
    ---@type GameStateMachineService
    local GameStateMachineService = GetGameService(self, GameServiceNameDef.GameStateMachineService)
    if GameStateMachineService then
        GameStateMachineService:ExitState(GameLuaStateNameDef.StatePreparationRoom)
    end
end

return PreparationRoomView
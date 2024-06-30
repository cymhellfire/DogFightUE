---@class PreparationRoomView : ModelBase, BP_Widget_PreparationRoom_C
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

---@param self PreparationRoomView
local function SyncPreviewWithSelection(self)

    local Item = self.CharacterSelect_ComboBox:GetSelectedOption()
    self.ViewModel.CharacterName = GetLocalizedString(LocalizationTable.Character, Item)

    local Index = self.CharacterSelect_ComboBox:GetSelectedIndex()
    self:PreviewAvatar(Index + 1)
end

function PreparationRoomView:OnShow()
    ---@type TimerService
    local TimerService = GetGameService(self, GameServiceNameDef.TimerService)
    if TimerService then
        self.OnShowTimer = TimerService:RegisterTimer(self, self.OnShowTimerExpired, 0.1)
    end
end

function PreparationRoomView:OnShowTimerExpired()
    -- Sync with default selection
    SyncPreviewWithSelection(self)
end

function PreparationRoomView:OnCharacterSelectChanged(Item, SelectionType)
    print("PreparationRoomView:OnCharacterSelectChanged ", Item)
    -- Sync preview
    SyncPreviewWithSelection(self)
end

function PreparationRoomView:OnBackButtonClicked()
    ---@type GameStateMachineService
    local GameStateMachineService = GetGameService(self, GameServiceNameDef.GameStateMachineService)
    if GameStateMachineService then
        GameStateMachineService:ExitState(GameLuaStateNameDef.StatePreparationRoom)
    end
end

---Preview avatar by given config.
function PreparationRoomView:PreviewAvatar(InConfigId)
    ---@type StatePreparationRoom
    local MyState
    ---@type GameStateMachineService
    local GameStateMachineService = GetGameService(self, GameServiceNameDef.GameStateMachineService)
    if GameStateMachineService then
        MyState = GameStateMachineService:GetActiveStateByName(GameLuaStateNameDef.StatePreparationRoom)
    end

    -- Skip if current state missing
    if not MyState then
        return
    end

    UE.UAvatarFunctionLibrary.InitAvatarAppearanceWithConfigId(self, MyState.Character, InConfigId)
end

return PreparationRoomView
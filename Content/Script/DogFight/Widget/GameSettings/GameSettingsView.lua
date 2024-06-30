---@class GameSettingsView : BP_Widget_GameSettings_C 
local GameSettingsView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local GameSettingsVM = require("DogFight.Widget.GameSettings.GameSettingsVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")
local GameLuaStateNameDef = require("DogFight.Services.GameStateMachineService.GameLuaStateNameDef")

function GameSettingsView:PostInitialized()
    local NewVM = InstantiateViewModel(GameSettingsVM)
    self:BindViewModel(NewVM, {
        --{BindKey = "TitleMessage",   UIKey = "TitleMessage_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })

    ---@type ListViewWrapper
    self.PlayerInfoList = ListWrapper.New(self, self.PlayerInfo_ListView)

    self.BackButton.OnClicked:Add(self, self.OnBackButtonClicked)
end

function GameSettingsView:UnInitialize()
    self.BackButton.OnClicked:Clear()
end

---@param self GameSettingsView
local function InitPlayerInfo(self)
    local PropertyDataList = {}
    -- Player Name
    local PlayerName = {
        Name = GetLocalizedString(LocalizationTable.CommonUI, "UI_PlayerName"),
        Value = UE.UGameSettingsFunctionLibrary.GetPlayerName(),
        bEditable = true,
        bCheckEmpty = true,
        OnChangeCallback = function(InValue)
            UE.UGameSettingsFunctionLibrary.SetPlayerName(InValue)
        end,
    }
    PropertyDataList[#PropertyDataList + 1] = PlayerName

    self.PlayerInfoList:LoadDataByList(PropertyDataList)
end

function GameSettingsView:OnShow()
    InitPlayerInfo(self)
end

function GameSettingsView:OnBackButtonClicked()
    -- Save global settings before leave
    UE.UGameSettingsFunctionLibrary.SaveGameSettings(self)

    ---@type GameStateMachineService
    local GameStateMachineService = GetGameService(self, GameServiceNameDef.GameStateMachineService)
    if GameStateMachineService then
        GameStateMachineService:TryEnterState(GameLuaStateNameDef.StateMainMenu)
    end
end

return GameSettingsView
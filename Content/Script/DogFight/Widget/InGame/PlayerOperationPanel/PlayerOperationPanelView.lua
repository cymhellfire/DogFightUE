---@class PlayerOperationPanelView : BP_Widget_PlayerOperationPanel_C 
local PlayerOperationPanelView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local PlayerOperationPanelVM = require("DogFight.Widget.InGame.PlayerOperationPanel.PlayerOperationPanelVM")
--local ListWrapper = require("Common.ListView.ListViewWrapper")

function PlayerOperationPanelView:Initialize()
    local NewVM = InstantiateViewModel(PlayerOperationPanelVM)
    self:BindViewModel(NewVM, {
        --{BindKey = "TitleMessage",   UIKey = "TitleMessage_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })
end

function PlayerOperationPanelView:PostInitialized()
    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)

    self.FinishRound_Button.OnClicked:Add(self, self.OnFinishRoundButtonClicked)
end

function PlayerOperationPanelView:OnFinishRoundButtonClicked()
    -- Request finish round immediately
    UE.UCommonGameFlowFunctionLibrary.RequestFinishLocalPlayerRound(self)
end

return PlayerOperationPanelView
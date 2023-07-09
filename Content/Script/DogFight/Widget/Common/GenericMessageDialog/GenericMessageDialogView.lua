---@class GenericMessageDialogView : BP_Widget_GenericMessageDialog_C 
local GenericMessageDialogView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local GenericMessageDialogVM = require("DogFight.Widget.Common.GenericMessageDialog.GenericMessageDialogVM")
--local ListWrapper = require("Common.ListView.ListViewWrapper")

local GameWidgetNameDef = require("DogFight.Services.GameWidgetService.GameWidgetNameDef")

function GenericMessageDialogView:PostInitialized()
    local NewVM = InstantiateViewModel(GenericMessageDialogVM)
    self:BindViewModel(NewVM, {
        {BindKey = "Title",     UIKey = "Title_Text",       DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "Content",   UIKey = "Content_Text",     DataBinding = DataBinding.TextContextBinding() },
    })

    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)

    self.Confirm_Button.OnClicked:Add(self, self.OnConfirmButtonClicked)
end

function GenericMessageDialogView:UnInitialize()
    self.Confirm_Button.OnClicked:Remove(self, self.OnConfirmButtonClicked)
end

---@param InMessage FGenericMessage
function GenericMessageDialogView:OnShow(InMessage)
    self.ViewModel.Title = GetLocalizedString(LocalizationTable.CommonUI, InMessage.Title)
    self.ViewModel.Content = GetLocalizedString(LocalizationTable.CommonUI, InMessage.Content)
end

function GenericMessageDialogView:OnConfirmButtonClicked()
    ---@type GameWidgetService
    local GameWidgetService = GetGameService(self, GameServiceNameDef.GameWidgetService)
    if GameWidgetService then
        GameWidgetService:RemoveWidget(GameWidgetNameDef.WidgetGenericMessageDialog)
    end
end

return GenericMessageDialogView
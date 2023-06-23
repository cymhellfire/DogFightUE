---@class InGameChatItemView : ModelBase 
local InGameChatItemView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local InGameChatItemVM = require("DogFight.Widget.Common.InGameChat.InGameChatItemVM")
--local ListWrapper = require("Common.ListView.ListViewWrapper")

function InGameChatItemView:PostInitialized()
    local NewVM = InstantiateViewModel(InGameChatItemVM)
    self:BindViewModel(NewVM, {
        {BindKey = "Source",    UIKey = "Source_Text",      DataBinding = DataBinding.TextContextBinding(), },
        {BindKey = "Content",   UIKey = "Content_Text",     DataBinding = DataBinding.TextContextBinding(), },
    })
    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)
end

function InGameChatItemView:OnListItemObjectSet(InObject)
    if InObject and InObject["GetData"] then
        ---@type FInGameChatMessage
        local MsgData = InObject:GetData()
        if MsgData then

            self.ViewModel.Source = (MsgData.SourceType == UE.EInGameChatSourceType.System) and "System" or ("Player" .. MsgData.SourcePlayerId)
            self.ViewModel.Content = MsgData.Content
        end
    end
end

return InGameChatItemView
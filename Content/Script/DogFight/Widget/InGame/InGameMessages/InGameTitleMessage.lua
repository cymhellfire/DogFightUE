require "UnLua"

local InGameTitleMessage = Class("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local InGameTitleMessageVM = require("DogFight.Widget.InGame.InGameMessages.InGameTitleMessageVM")

function InGameTitleMessage:Initialize()
    local NewVM = InstantiateViewModel(InGameTitleMessageVM)
    self:BindViewModel(NewVM, {
        {BindKey = "TitleMessage",   UIKey = "TitleMessage_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })
end

function InGameTitleMessage:PostInitialized()
    GetGameService(self, GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_ReceiveTitleMessage, self, self.OnReceiveTitleMessage)
end

function InGameTitleMessage:OnReceiveTitleMessage(InMsg)
    --self.TitleMessage_Text:SetText(InMsg)
    self.ViewModel.TitleMessage = InMsg
end

return InGameTitleMessage

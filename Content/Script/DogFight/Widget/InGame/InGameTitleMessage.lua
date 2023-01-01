require "UnLua"

local InGameTitleMessage = Class("Common.MVVM.ModelBase")
local InGameTitleMessageVM = require("DogFight.Widget.InGame.InGameTitleMessageVM")
local DataBinding = require("Common.MVVM.DataBinding")

function InGameTitleMessage:Initialize()
    self:BindViewModel(InGameTitleMessageVM, {
        {BindKey = "TitleMessage",   UIKey = "TitleMessage_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })

    GameServices.LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_ReceiveTitleMessage, self, self.OnReceiveTitleMessage)
end

function InGameTitleMessage:OnReceiveTitleMessage(InMsg)
    --self.TitleMessage_Text:SetText(InMsg)
    self.ViewModel.TitleMessage = InMsg
end

return InGameTitleMessage

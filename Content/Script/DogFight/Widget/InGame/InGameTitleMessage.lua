require "UnLua"

local InGameTitleMessage = Class()

function InGameTitleMessage:Initialize()
    GameServices.LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_ReceiveTitleMessage, self, self.OnReceiveTitleMessage)
end

function InGameTitleMessage:OnReceiveTitleMessage(InMsg)
    self.TitleMessage_Text:SetText(InMsg)
end

return InGameTitleMessage

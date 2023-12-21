---@class InGameChatView : BP_Widget_InGameChat_C, ModelBase
local InGameChatView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local InGameChatVM = require("DogFight.Widget.Common.InGameChat.InGameChatVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")

function InGameChatView:PostInitialized()
    local NewVM = InstantiateViewModel(InGameChatVM)
    self:BindViewModel(NewVM, {
        --{BindKey = "TitleMessage",   UIKey = "TitleMessage_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })

    ---@type ListViewWrapper 
    self.ChatMsgList = ListWrapper.New(self, self.ChatMsgList_ListView)

    self.ChatInput_TextBox.OnTextCommitted:Add(self, self.OnTextCommitted)
    self.Send_Button.OnClicked:Add(self, self.OnSendClicked)

    print("InGameChatView:PostInitialized with " .. UE.UCommonGameFlowFunctionLibrary.GetLocalPlayerId(self))

    ---@type LuaEventService
    local LuaEventService = GetGameService(self, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_ReceiveInGameChatMessage, self, self.OnReceiveInGameMessage)
    end
end

function InGameChatView:OnTextCommitted(InText, Type)
    self:OnSendClicked()
end

function InGameChatView:OnSendClicked()
    local InputText = self.ChatInput_TextBox:GetText()
    if type(InputText) == "string" and #InputText > 0 then
        self:SendChatMessage(InputText)
    end
end

function InGameChatView:SendChatMessage(InContent)
    local MyPlayerId = UE.UCommonGameFlowFunctionLibrary.GetLocalPlayerId(self)
    local SendOption = UE.FInGameChatSendOption()
    SendOption.SourcePlayerId = MyPlayerId
    SendOption.Content = InContent

    UE.UInGameMessageFunctionLibrary.SendInGameChatMessageAsPlayer(self, SendOption)

    -- Clear the input box after send
    self.ChatInput_TextBox:SetText("")
    -- Set a timer for focus
    ---@type TimerService
    local TimerService = GetGameService(self, GameServiceNameDef.TimerService)
    if TimerService then
        TimerService:RegisterTimer(self, self.DelayRecoverFocus, 0.1)
    end
end

function InGameChatView:DelayRecoverFocus()
    -- Set the focus back
    self.ChatInput_TextBox:SetKeyboardFocus()
end

function InGameChatView:OnReceiveInGameMessage()
    -- Get the chat message from receiver
    local Receiver = UE.UInGameMessageFunctionLibrary.GetLocalPlayerMessageReceiver(self)
    if Receiver then
        local bHasMessage, NewMessage = Receiver:GetCachedGameChatMessage()
        if bHasMessage then
            self.ChatMsgList:AddDataObject(NewMessage)

            -- Scroll to the bottom
            self.ChatMsgList_ListView:ScrollToBottom()
        end
    end
end

return InGameChatView
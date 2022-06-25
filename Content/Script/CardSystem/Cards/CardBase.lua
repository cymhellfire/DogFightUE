require("UnLua")

local CardBase = Class()

function CardBase:RegisterCallback(CommandIndex, Callback)
    if self.CallbackMap == nil then
        self.CallbackMap = {}
    end

    self.CallbackMap[CommandIndex] = Callback
end

function CardBase:OnCallbackResult(CommandIndex, Result)
    if self.CallbackMap == nil or self.CallbackMap[CommandIndex] == nil then
        return
    end

    self.CallbackMap[CommandIndex](self, Result)
end

return CardBase

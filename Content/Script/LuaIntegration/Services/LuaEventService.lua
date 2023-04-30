require "Common.TableUtils"

---@class LuaEventService
local LuaEventService = UnrealClass("DogFight.Services.GameServiceBase")

function LuaEventService:StartupScript(ServiceName)
    self.Super.StartupScript(self, ServiceName)

    self.EventDelegateMap = {}
end

function LuaEventService:TriggerDelegate(EventIndex, ...)
    local Delegate = self.EventDelegateMap[EventIndex]
    if Delegate then
        Delegate:Trigger(...)
    end
end

function LuaEventService:RegisterListener(EventIndex, Object, Callback)
    -- Create new delegate for event
    if not table.containsKey(self.EventDelegateMap, EventIndex) then
        local EventDelegate = require("LuaIntegration.Common.EventDelegate")
        self.EventDelegateMap[EventIndex] = EventDelegate:New()
    end

    local Delegate = self.EventDelegateMap[EventIndex]
    Delegate:Add(Object, Callback)
end

function LuaEventService:UnregisterListener(EventIndex, Object, Callback)
    if not table.containsKey(self.EventDelegateMap, EventIndex) then
        return
    end

    local Delegate = self.EventDelegateMap[EventIndex]
    Delegate:Remove(Object, Callback)
end

function LuaEventService:OnReceiveNativeNotify(EventIndex, ...)
    print(self, ":OnReceiveNativeNotify ", EventIndex, " Params: ", ...)
    self:TriggerDelegate(EventIndex, ...)
end

return LuaEventService

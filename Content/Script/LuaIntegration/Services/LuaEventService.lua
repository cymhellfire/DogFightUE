require "UnLua"
require "Common.TableUtils"

local LuaEventService = Class("DogFight.Services.GameServiceBase")

function LuaEventService:StartupScript(ServiceName)
    self.Super.StartupScript(self, ServiceName)

    self.EventDelegateMap = {}
end

function LuaEventService:SendEventToLua(InEvent)
    self:TriggerDelegate(InEvent)
end

function LuaEventService:SendEventToLua_OneParam(InEvent, InParam)
    self:TriggerDelegate(InEvent, InParam)
end

function LuaEventService:TriggerDelegate(EventName, ...)
    local Delegate = self.EventDelegateMap[EventName]
    if Delegate then
        Delegate:Trigger(...)
    end
end

function LuaEventService:RegisterListener(EventName, Object, Callback)
    -- Create new delegate for event
    if not table.containsKey(self.EventDelegateMap, EventName) then
        local EventDelegate = require("LuaIntegration.Common.EventDelegate")
        self.EventDelegateMap[EventName] = EventDelegate:New()
    end

    local Delegate = self.EventDelegateMap[EventName]
    Delegate:Add(Object, Callback)
end

function LuaEventService:UnregisterListener(EventName, Object, Callback)
    if not table.containsKey(self.EventDelegateMap, EventName) then
        return
    end

    local Delegate = self.EventDelegateMap[EventName]
    Delegate:Remove(Object, Callback)
end

return LuaEventService

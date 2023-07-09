require "Common.TableUtils"

---@class LuaEventService : ULuaEventService
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
    local EventName = UE.ELuaEvent.GetNameStringByValue(EventIndex) or EventIndex
    print(self, ":OnReceiveNativeNotify ", EventName, " Params: ", ...)
    self:TriggerDelegate(EventIndex, ...)
end

return LuaEventService

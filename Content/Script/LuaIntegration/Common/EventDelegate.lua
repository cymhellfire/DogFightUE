require "UnLua"
require "Common.TableUtils"

local EventDelegate = Class()

function EventDelegate:ctor()
    self.ListenerMap = {}
end

function EventDelegate:Add(Object, Callback)
    if not Object or not Callback then
        return
    end

    -- Add new map for object
    if not table.containsKey(self.ListenerMap, Object) then
        self.ListenerMap[Object] = {}
    end

    local ObjectCallbackMap = self.ListenerMap[Object]
    if table.containsValue(ObjectCallbackMap, Callback) then
        print("EventDelegate: Duplicate callback registry.")
        return
    end
    ObjectCallbackMap[#ObjectCallbackMap + 1] = Callback
end

function EventDelegate:Remove(Object, Callback)
    if not table.containsKey(self.ListenerMap, Object) then
        return
    end

    local ObjectCallbackMap = self.ListenerMap[Object]
    if not table.containsValue(ObjectCallbackMap, Callback) then
        return
    end
    table.removeValue(ObjectCallbackMap, Callback)

    -- Remove empty callback map
    if #ObjectCallbackMap == 0 then
        self.ListenerMap[Object] = nil
    end
end

function EventDelegate:Trigger(...)
    -- Notify all listener
    for Object, Map in pairs(self.ListenerMap) do
        if #Map > 0 then
            for _, Callback in ipairs(Map) do
                pcall(Callback, Object, ...)
            end
        end
    end
end

return EventDelegate

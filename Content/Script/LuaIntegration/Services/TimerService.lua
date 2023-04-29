---@class TimerService Service that manage all timer.
local TimerService = UnrealClass("DogFight.Services.GameServiceBase")

---Register a timer which can trigger once or multiple times.
---@param InObj table Callback function container.
---@param InCallback function Callback function to trigger.
---@param Interval number How often this callback is triggered?
---@param bIsLoop boolean Is the timer looping
---@return FTimerHandle Handle of new register timer
function TimerService:RegisterTimer(InObj, InCallback, Interval, bIsLoop)
    if InObj == nil or InCallback == nil then
        return
    end

    local TimerHandle = UE.UKismetSystemLibrary.K2_SetTimerDelegate({self, function()
        -- Invoke callback
        InCallback(InObj)
    end}, Interval, bIsLoop or false)

    return TimerHandle
end

---Clear specified timer by handle.
---@param InHandle FTimerHandle Handle of timer to unregister.
function TimerService:UnregisterTimer(InHandle)
    if InHandle == nil then
        return
    end

    UE.UKismetSystemLibrary.K2_ClearTimerHandle(self, InHandle)
end

return TimerService
---@field _NativeDelegate MulticastDelegate Pointer of native delegate this instance bound to.
---@field _Listener any Listener instance holds the callback function.
---@field _Callback function Callback function.
---@class DelegateHelper : UDelegateHelper Helper class for non-bound lua script to listen native delegate.
local DelegateHelper = UnrealClass()

function DelegateHelper:OnReset()
    -- Unbind the delegate when reset
    self:UnbindDelegate()
end

---Bind a callback function to specified delegate.
---@param InDelegate MulticastDelegate Native delegate pointer.
---@param InListener any Listener instance that holds the callback function.
---@param InCallback function Callback function to trigger.
function DelegateHelper:BindDelegate(InDelegate, InListener, InCallback)
    if not InDelegate then
        return
    end
    InDelegate:Add(self, self.OnNativeDelegateTriggered)

    self._NativeDelegate = InDelegate
    self._Listener = InListener
    self._Callback = InCallback
end

---Unbind the callback from native delegate.
function DelegateHelper:UnbindDelegate()
    if not self._NativeDelegate then
        return
    end

    self._NativeDelegate:Remove(self, self.OnNativeDelegateTriggered)
end

---Callback function that response to native delegate.
---And pass the parameters to lua callback.
function DelegateHelper:OnNativeDelegateTriggered(...)
    if self._Listener and self._Callback then
        self._Callback(self._Listener, ...)
    end
end

return DelegateHelper
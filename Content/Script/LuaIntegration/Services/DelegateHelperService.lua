---@class DelegateHelperService : UDelegateHelperService Service that holds all delegate helper relative functions.
local DelegateHelperService = Class("DogFight.Services.GameServiceBase")

---Bind a callback function to specified native delegate.
---@param InDelegate MulticastDelegate Native delegate bind callback to.
---@param InListener any Object that holds the callback function.
---@param InCallback function Callback function.
---@return UDelegateHelper Helper instance.
function DelegateHelperService:BindCallback(InDelegate, InListener, InCallback)
    ---@type DelegateHelper
    local NewHelper = self:GetDelegateHelper()
    if NewHelper then
        NewHelper:BindDelegate(InDelegate, InListener, InCallback)
    end

    return NewHelper
end

return DelegateHelperService
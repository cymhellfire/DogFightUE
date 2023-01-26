require "LuaIntegration.Common.UnrealUtils"

---Get the game service with given name.
---@param InName string Name of game service to find.
---@return UGameService Found service.
local function GetGameService(InName)
    ---@type UDogFightGameInstance
    local GameInstance = GetGameInstance()
    if GameInstance then
        return GameInstance:GetGameService(InName)
    end

    return nil
end

_G.GetGameService = GetGameService
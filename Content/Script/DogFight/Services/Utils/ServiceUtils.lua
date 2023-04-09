require "LuaIntegration.Common.UnrealUtils"

---Get the game service with given name.
---@param WorldContextObject UObject Object instance to get world with.
---@param InName string Name of game service to find.
---@return UGameService Found service.
local function GetGameService(WorldContextObject, InName)
    ---@type UDogFightGameInstance
    local GameInstance = GetGameInstance(WorldContextObject)
    if GameInstance then
        return GameInstance:GetGameService(InName)
    end

    return nil
end

_G.GetGameService = GetGameService
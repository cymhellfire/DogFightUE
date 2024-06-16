---@field bSessionReady boolean Indicate the session is ready.
---@class StateGameLobby : GameLuaState State of game lobby
local StateGameLobby = UnrealClass("DogFight.Services.GameStateMachineService.GameLuaState")

---@param self StateGameLobby
local function CheckWaitingSession(self)
    if type(self.ExtraArgs) == "table" then
        return self.ExtraArgs.bWaitingSession
    end
    return false
end

function StateGameLobby:InitWithConifg(InConfig, InService)
    self.Super.InitWithConifg(self, InConfig, InService)

    self.bSessionReady = false
    ---@type LuaEventService
    local LuaEventService = GetGameService(InService, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_GameLobbyLevelLoaded, self, self.OnGameLobbyLevelLoaded)
    end
end

function StateGameLobby:CheckEnterCondition()
    if CheckWaitingSession(self) then
        return self.bSessionReady
    end
    return true
end

function StateGameLobby:OnGameLobbyLevelLoaded()
    print("StateGameLobby:OnGameLobbyLevelLoaded")
    self.bSessionReady = true
end

function StateGameLobby:OnExit()
    self.Super.OnExit(self)

    self.bSessionReady = false
end

return StateGameLobby
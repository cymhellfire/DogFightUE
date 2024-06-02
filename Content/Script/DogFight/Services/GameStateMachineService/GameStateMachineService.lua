
local GameLuaStateNameDef = require("DogFight.Services.GameStateMachineService.GameLuaStateNameDef")
---@field StateMap table<GameLuaStateNameDef, GameLuaState> Table contains all instantiated lua game state.
---@field StateStack list<GameLuaState> Stack to maintain all active states.
---@class GameStateMachineService : GameServiceBase, UGameStateMachineService Service to manage all game states.
local GameStateMachineService = UnrealClass("DogFight.Services.GameServiceBase")

function GameStateMachineService:StartupScript(ServiceName)
    self.Super.StartupScript(self, ServiceName)

    self.StateStack = {}
    self:InitAllStates()
end

function GameStateMachineService:ShutdownScript()
    self.Super.ShutdownScript(self)

    ---@type LuaEventService
    local LuaEventService = GetGameService(self, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_GameStartup, self, self.OnGameStartup)
    end
end

function GameStateMachineService:PostStartupScript()
    -- Listen to game startup event
    ---@type LuaEventService
    local LuaEventService = GetGameService(self, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_GameStartup, self, self.OnGameStartup)
    end
end

function GameStateMachineService:OnGameStartup()
    print("GameStateMachineService:OnGameStartup")
    self:TryEnterState(GameLuaStateNameDef.StateMainMenu)
end

function GameStateMachineService:GetConfigPath()
    return "DogFight.Services.GameStateMachineService.GameLuaStateConfig"
end

---Instantiated all game states.
function GameStateMachineService:InitAllStates()
    self.StateMap = {}
    for k, v in pairs(self.Config.Data) do
        if type(v.Script) == "string" and #v.Script > 0 then
            v.Name = k
            -- Fill short map name
            if type(v.LoadMap) == "string" then
                v.ShortMapName = self:GetShortMapName(v.LoadMap)
            end
            local StateClass = require(v.Script)
            ---@type GameLuaState
            local NewState = StateClass:New()
            -- Initialize
            NewState:InitWithConifg(v, self)
            self.StateMap[k] = NewState
            print("GameStateMachineService:InitAllStates New state created ", k)
        else
            warn("GameStateMachineService:InitAllStates Invalid state script ", k)
        end
    end
end

---Push given state to the stack
---@param self GameStateMachineService
---@param NewState GameLuaState
local function PushState(self, NewState)
    if table.containsValue(self.StateStack, NewState) then
        error("Duplicated game state in stack ", NewState.Name)
        return
    end

    self.StateStack[#self.StateStack + 1] = NewState
end

---Pop state on the stack top.
---@param self GameStateMachineService
---@return GameLuaState State on the top.
local function PopState(self)
    if #self.StateStack == 0 then
        error("Empty state stack to pop.")
        return
    end

    local TopState = self.StateStack[#self.StateStack]
    self.StateStack[#self.StateStack] = nil
    return TopState
end

---Enter the state on stack top.
---@param self GameStateMachineService
local function EnterStackTopState(self)
    ---@type GameLuaState
    local CurState = self:GetCurrentState()
    if not CurState then
        return
    end

    -- Start switch process
    CurState:PreEnter()

    if CurState.LoadMap then
        -- Load map and wait
        UE.UFrameworkLibrary.LoadGameMap(self, CurState.LoadMap)
        self.PendingState = CurState
        return
    end

    -- Enter the state
    CurState:OnEnter()
end

---Triggered after new map loaded.
---@param InMapName string Name of new loaded map.
function GameStateMachineService:OnPostLoadMapScript(InMapName)
    -- Check if the map is loaded by pending state
    if self.PendingState and type(self.PendingState.ShortMapName) == "string" then
        if InMapName == self.PendingState.ShortMapName then
            self.PendingState:OnEnter()
            self.PendingState = nil
        end
    end
end

---Get current state
---@return GameLuaState Current state
function GameStateMachineService:GetCurrentState()
    return (#self.StateStack > 0) and self.StateStack[#self.StateStack]
end

---Try to enter specified game state.
---@param InStateName GameLuaStateNameDef Name of state to enter.
function GameStateMachineService:TryEnterState(InStateName)
    local State = self.StateMap[InStateName]
    if not State then
        error("GameStateMachineService:TryEnterState State ", InStateName, " not found.")
    end

    -- Clear all states between last entered state and current one
    -- This helps to keep the state stack simple and clean
    if table.containsValue(self.StateStack, State) then
        -- Remove from end
        for i = #self.StateStack, 1, -1 do
            -- Stop when find target state
            if self.StateStack[i] == State then
                break
            end
            self.StateStack[i] = nil
        end
    end

    ---@type GameLuaState Current state
    local CurState = self:GetCurrentState()

    -- Exit current state
    if CurState then
        CurState:OnExit()
    end

    -- Directly enter state if no map to load
    PushState(self, State)
    EnterStackTopState(self)
end

---Exit specified state if current in it.
---@param InStateName string Name of state to exit.
function GameStateMachineService:ExitState(InStateName)
    ---@type GameLuaState
    local TargetState = self.StateMap[InStateName]
    if not TargetState then
        error("GameStateMachineService:ExitState Invalid state name: ", InStateName)
        return
    end

    ---@type GameLuaState
    local CurState = self:GetCurrentState()
    if not CurState then
        error("GameStateMachineService:ExitState No state to exit.")
        return
    end
    local bIsTopState = CurState == TargetState

    if not table.containsValue(self.StateStack, TargetState) then
        warn("GameStateMachineService:ExitState State ", InStateName, " not in stack.")
        return
    end

    -- Exit current state
    if bIsTopState then
        TargetState:OnExit()

        PopState(self)
        EnterStackTopState(self)
    else
        -- Remove target state
        table.removeValue(self.StateStack, TargetState)
    end
end

return GameStateMachineService
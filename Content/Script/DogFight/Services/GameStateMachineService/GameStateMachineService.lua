
local GameLuaStateNameDef = require("DogFight.Services.GameStateMachineService.GameLuaStateNameDef")
local BitField = require("Common.BitField")
---@field StateMap table<GameLuaStateNameDef, GameLuaState> Table contains all instantiated lua game state.
---@field StateStack list<GameLuaState> Stack to maintain all active states.
---@field PendingStateFlag number Bit field that represents the pending state waiting condition.
---@class GameStateMachineService : GameServiceBase, UGameStateMachineService Service to manage all game states.
local GameStateMachineService = UnrealClass("DogFight.Services.GameServiceBase")

local GameStatePendingCondition = {
    None = 0,
    MapLoading = 1,
    StateNotReady = 2,
}

function GameStateMachineService:StartupScript(ServiceName)
    self.Super.StartupScript(self, ServiceName)

    self.StateStack = {}
    self.PendingStateFlag = GameStatePendingCondition.None
    self:InitAllStates()
end

function GameStateMachineService:ShutdownScript()
    self.Super.ShutdownScript(self)

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

    ---Unregister event since this event is only used for initialize first state
    ---@type LuaEventService
    local LuaEventService = GetGameService(self, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_GameStartup, self, self.OnGameStartup)
    end
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
        print("Duplicated game state in stack ", NewState.Name)
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

    -- Check if new state is ready to enter
    local bReady = CurState:CheckEnterCondition()
    if not bReady then
        self.PendingStateFlag = BitField.AddBit(self.PendingStateFlag, GameStatePendingCondition.StateNotReady)
    end

    if CurState.LoadMap then
        -- Only start loading when different map is given
        if not UE.UFrameworkLibrary.IsCurrentMap(self, CurState.LoadMap) then
            -- Load map and wait
            UE.UFrameworkLibrary.LoadGameMap(self, CurState.LoadMap)
            self.PendingStateFlag = BitField.AddBit(self.PendingStateFlag, GameStatePendingCondition.MapLoading)
        end
    end

    if self.PendingStateFlag == GameStatePendingCondition.None then
        -- Enter the state
        CurState:OnEnter()
    else
        -- Record pending state
        self.PendingState = CurState
    end
end

---Check if pending game state condition is all completed.
---@param self GameStateMachineService
local function CheckPendingStateCondition(self)
    -- Check pending condition
    if self.PendingStateFlag == GameStatePendingCondition.None then
        self.PendingState:OnEnter()
        self.PendingState = nil
    end
end

---Triggered after new map loaded.
---@param InMapName string Name of new loaded map.
function GameStateMachineService:OnPostLoadMapScript(InMapName)
    -- Check if the map is loaded by pending state
    if self.PendingState and type(self.PendingState.LoadMap) == "string" then
        if InMapName == self.PendingState.LoadMap then
            -- Remove condition
            self.PendingStateFlag = BitField.RemoveBit(self.PendingStateFlag, GameStatePendingCondition.MapLoading)
            CheckPendingStateCondition(self)
        end
    end
end

---Get current state
---@return GameLuaState Current state
function GameStateMachineService:GetCurrentState()
    return (#self.StateStack > 0) and self.StateStack[#self.StateStack]
end

---Get activated state by name.
---@return GameLuaState Actived state.
function GameStateMachineService:GetActiveStateByName(InStateName)
    local TargetState = self.StateMap[InStateName]
    if TargetState then
        -- Check if the target state is currently activated
        if TargetState == self:GetCurrentState() then
            return TargetState
        end
    end
end

---Try to enter specified game state.
---@param InStateName GameLuaStateNameDef Name of state to enter.
---@param ExtraArgs table Extra argument table for new state.
function GameStateMachineService:TryEnterState(InStateName, ExtraArgs)
    local State = self.StateMap[InStateName]
    if not State then
        error("GameStateMachineService:TryEnterState State ", InStateName, " not found.")
    end

    -- Set the extra arguments
    State.ExtraArgs = ExtraArgs

    ---@type GameLuaState Current state
    local CurState = self:GetCurrentState()

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

---@param DeltaTime number
function GameStateMachineService:OnTickScript(DeltaTime)
    -- Check pending state every tick
    if self.PendingState and self.PendingStateFlag ~= GameStatePendingCondition.None then
        local bReady = self.PendingState:CheckEnterCondition()

        if bReady then
            self.PendingStateFlag = BitField.RemoveBit(self.PendingStateFlag, GameStatePendingCondition.StateNotReady)
            CheckPendingStateCondition(self)
        end
    end
end

return GameStateMachineService
---@field Name string Name of this state.
---@field LoadMap string Map to load when enter state.
---@field ShortMapName string Short version of map this state load.
---@field StateMachine GameStateMachineService Owner state machine service.
---@class GameLuaState Base class of all lua states.
local GameLuaState = UnrealClass()

---Initialize state with given config
---@param InConfig table State config
---@param InService GameStateMachineService Owner service
function GameLuaState:InitWithConifg(InConfig, InService)
    self.Name = InConfig.Name
    self.LoadMap = InConfig.LoadMap
    self.ShortMapName = InConfig.ShortMapName
    self.StateMachine = InService
end

function GameLuaState:PreEnter()
    print("GameLuaState:PreEnter", self.Name)
end

function GameLuaState:OnEnter()
    print("GameLuaState:OnEnter", self.Name)
end

function GameLuaState:OnExit()
    print("GameLuaState:OnExit")
end

return GameLuaState
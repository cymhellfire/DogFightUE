---@field Name string Name of this state.
---@field LoadMap string Map to load when enter state.
---@field InitialWidgets list<string> List of widget name that automatically create when enter the state.
---@field StateMachine GameStateMachineService Owner state machine service.
---@field ExtraArgs table Extra arguments used in generic purposes.
---@class GameLuaState Base class of all lua states.
local GameLuaState = UnrealClass()

---Initialize state with given config
---@param InConfig table State config
---@param InService GameStateMachineService Owner service
function GameLuaState:InitWithConifg(InConfig, InService)
    self.Name = InConfig.Name
    self.LoadMap = InConfig.LoadMap
    self.InitialWidgets = InConfig.InitialWidgets
    self.StateMachine = InService
end

---Check if the game state is ready to enter.
---@return boolean Is ready to enter?
function GameLuaState:CheckEnterCondition()
    return true
end

---Check if the map loading should be skipped.
---@return boolean Should the load map be skipped?
function GameLuaState:SkipLoadMap()
    return type(self.ExtraArgs) == "table" and self.ExtraArgs.bSkipLoadMap
end

function GameLuaState:OnEnter()
    print("GameLuaState:OnEnter", self.Name)

    -- Create initial widgets
    if type(self.InitialWidgets) == "table" and #self.InitialWidgets > 0 then
        ---@type GameWidgetService
        local GameWidgetService = GetGameService(self.StateMachine, GameServiceNameDef.GameWidgetService)
        if GameWidgetService then
            for _, v in ipairs(self.InitialWidgets) do
                GameWidgetService:ShowWidget(v, true)
            end
        end
    end
end

function GameLuaState:OnExit()
    print("GameLuaState:OnExit", self.Name)

    -- Remove initial widgets
    if type(self.InitialWidgets) == "table" and #self.InitialWidgets > 0 then
        ---@type GameWidgetService
        local GameWidgetService = GetGameService(self.StateMachine, GameServiceNameDef.GameWidgetService)
        if GameWidgetService then
            for _, v in ipairs(self.InitialWidgets) do
                GameWidgetService:RemoveWidget(v)
            end
        end
    end
end

return GameLuaState
---@field Name string Name of this state.
---@field LoadMap string Map to load when enter state.
---@field ShortMapName string Short version of map this state load.
---@field InitialWidgets list<string> List of widget name that automatically create when enter the state.
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
    self.InitialWidgets = InConfig.InitialWidgets
    self.StateMachine = InService
end

function GameLuaState:PreEnter()
    print("GameLuaState:PreEnter", self.Name)
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
local CharacterPreviewStateMachine = UnrealClass()

local function InitStateMachineSettings(self)
    self.StateMachineSettings = {
        PlayerCount = 1,
        BotCount = 2,
    }
end

function CharacterPreviewStateMachine:K2_InitStateMachine()
    print("CharacterPreviewStateMachine:K2_InitStateMachine")
    InitStateMachineSettings(self)

    ---@type GameFlowStateService
    local GameFlowStateService = GetGameService(self, GameServiceNameDef.GameFlowStateService)
    if GameFlowStateService then
        local InitStateArgument = GameFlowStateService:GetGameFlowStateCreateArgument(self)
        InitStateArgument.StateName = "StandardMode.InitState"
        InitStateArgument.Instigator = self

        self:PushState(InitStateArgument)
    end
end

function CharacterPreviewStateMachine:GetStateMachineSettings()
    return self.StateMachineSettings
end

return CharacterPreviewStateMachine
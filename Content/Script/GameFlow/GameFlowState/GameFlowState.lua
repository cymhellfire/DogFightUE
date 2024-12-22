local json = require "Common.json"

---@field Logic GameFlowStateLogicBase Logic class of this game flow state.
---@class GameFlowState : UGameFlowStateBase Base class of all game flow state.
local GameFlowState = UnrealClass()

function GameFlowState:Initialize()
    self.LogicBasePath = "GameFlow.GameFlowState."
end

function GameFlowState:K2_OnEnter()
    if self.Logic and self.Logic["OnEnter"] then
        self.Logic:OnEnter()
    end
end

function GameFlowState:K2_OnExit()
    if self.Logic and self.Logic["OnExit"] then
        self.Logic:OnExit()
    end
end

function GameFlowState:K2_OnInterrupted()
    if self.Logic and self.Logic["OnInterrupted"] then
        self.Logic:OnInterrupted()
    end
end

function GameFlowState:K2_OnResume()
    if self.Logic and self.Logic["OnResume"] then
        self.Logic:OnResume()
    end
end

function GameFlowState:K2_Tick(DeltaTime)
    if self.Logic and self.Logic["Tick"] then
        self.Logic:Tick(DeltaTime)
    end
end

function GameFlowState:K2_OnCreateArgumentSet()
    local LogicPath = self.CreateArgument["StateName"]
    if LogicPath and LogicPath ~= "" then
        self:LoadLogic(LogicPath)
    else
        print("No state name specified.")
    end
end

function GameFlowState:LoadLogic(LogicPath)
    local Logic = require(self.LogicBasePath .. LogicPath)
    if Logic then
        self.Logic = Logic:New()
        -- Decode extra data
        local ExtraData
        if #self.CreateArgument.ExtraData > 0 then
            ExtraData = json.decode(self.CreateArgument.ExtraData)
        end
        -- Register state to logic
        self.Logic:Init(self, ExtraData)
    else
        print("Failed to load logic " .. LogicPath)
    end
end

return GameFlowState

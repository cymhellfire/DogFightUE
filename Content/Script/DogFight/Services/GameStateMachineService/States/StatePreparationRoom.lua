---@class StatePreparationRoom : GameLuaState State of preparation room.
---@field MiniScene AMiniScene Scene to hold characters.
---@field Character AActionGameCharacter Preview character instance.
local StatePreparationRoom = UnrealClass("DogFight.Services.GameStateMachineService.GameLuaState")

local MiniScenePath = "/Game/DogFightGame/Blueprints/MiniScene/BP_MiniScene_PreparationRoom"
local CharacterPath = "/Game/DogFightGame/Blueprints/Character/BP_Char_PreparationRoom"

function StatePreparationRoom:OnEnter()
    self.Super.OnEnter(self)

    ---Create MiniScene and character
    ---@type UWorld
    local CurWorld = GetCurrentWorld(self.StateMachine)
    if CurWorld then
        local MiniSceneClass = UE.ULuaIntegrationFunctionLibrary.LoadClassByPath(MiniScenePath)
        if MiniSceneClass then
            local SpawnTrans = UE.FTransform()
            local SpawnRot = UE.FRotator()
            SpawnRot.Yaw = 180
            SpawnTrans.Rotation = SpawnRot:ToQuat()
            self.MiniScene = CurWorld:SpawnActorEx(MiniSceneClass, SpawnTrans)
        end
        
        local CharacterClass = UE.ULuaIntegrationFunctionLibrary.LoadClassByPath(CharacterPath)
        if self.MiniScene and CharacterClass then
            self.Character = CurWorld:SpawnActorEx(CharacterClass, UE.FTransform())
            if self.Character then
                self.MiniScene:AddChildActor(self.Character, "NewSlot")
            end
        end
    end
end

function StatePreparationRoom:OnExit()
    self.Super.OnExit(self)

    ---Destroy MiniScene
    if self.MiniScene and self.MiniScene:IsValid() then
        self.MiniScene:K2_DestroyActor()
        self.MiniScene = nil
    end
end

return StatePreparationRoom

require "UnLua"

---@class GameInputService Service that handle game input mapping change operations.
local GameInputService = Class("DogFight.Services.GameServiceBase")

function GameInputService:StartupScript(ServiceName)
    self.Super.StartupScript(self, ServiceName)

    -- Intialize the resource path map
    self.ResourceMap = {}
    self.ResourceMap[UE.EInputMappingType.InputMapping_CameraMove] = "/Game/DogFightGame/Input/IMC_CameraMove.IMC_CameraMove"
    self.ResourceMap[UE.EInputMappingType.InputMapping_CharacterMove] = "/Game/DogFightGame/Input/IMC_CharacterMove.IMC_CharacterMove"
    self.ResourceMap[UE.EInputMappingType.InputMapping_CardUsing] = "/Game/DogFightGame/Input/IMC_CardUsing.IMC_CardUsing"
end

function GameInputService:BP_GetInputMappingPath(InType)
    return self.ResourceMap[InType] or ""
end

return GameInputService
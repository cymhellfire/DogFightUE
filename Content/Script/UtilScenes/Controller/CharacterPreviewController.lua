---@class CharacterPreviewController : ATopDownStylePlayerController
local CharacterPreviewController = UnrealClass()

function CharacterPreviewController:ReceiveBeginPlay()
    -- Spawn preview character when begin play
    UE.UCommonGameFlowFunctionLibrary.SpawnPlayerCharacterPawn(self)

    -- Add character movement input to all players
    GetGameService(self, GameServiceNameDef.GameInputService):MulticastAddInputMapping(UE.EInputMappingType.InputMapping_CharacterMove)

    -- Enable input for character control
    UE.UCommonGameFlowFunctionLibrary.SetCharacterMoveEnableForAllPlayers(self, true)
end

---Change the avatar config current character using.
---@param ConfigId number Config id
function CharacterPreviewController:ChangePreviewCharacter(ConfigId)
    local Character = self:GetCharacterPawn()
    if Character then
        UE.UAvatarFunctionLibrary.InitAvatarAppearanceWithConfigId(self, Character, ConfigId)
    end
end

return CharacterPreviewController
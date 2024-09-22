local GameWidgetNameDef = require("DogFight.Services.GameWidgetService.GameWidgetNameDef")
---@class CharacterPreviewController : ATopDownStylePlayerController
local CharacterPreviewController = UnrealClass()

function CharacterPreviewController:ReceiveBeginPlay()
    -- Show card list widget
    ---@type GameWidgetService
    local WidgetService = GetGameService(self, GameServiceNameDef.GameWidgetService)
    if WidgetService then
        WidgetService:ShowWidget(GameWidgetNameDef.WidgetCardList, true)
    end
end

---Change the avatar config current character using.
---@param ConfigId number Config id
function CharacterPreviewController:ChangePreviewCharacter(ConfigId)
    local Character = self:GetCharacterPawn()
    if Character then
        UE.UAvatarFunctionLibrary.InitAvatarAppearanceWithConfigId(Character, ConfigId)
    end
end

return CharacterPreviewController
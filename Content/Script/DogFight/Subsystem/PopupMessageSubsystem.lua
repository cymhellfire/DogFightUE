local GameWidgetNameDef = require("DogFight.Services.GameWidgetService.GameWidgetNameDef")

---@class PopupMessageSubsystem : LuaGameInstanceSubsystem
local PopupMessageSubsystem = UnrealClass("DogFight.Subsystem.LuaGameInstanceSubsystem")

function PopupMessageSubsystem:ScriptInit()
    self.Super.ScriptInit(self)


end

---@param InMessage FGenericMessage
function PopupMessageSubsystem:HandleMessagePopup(InMessage)
    ---@type GameWidgetService
    local GameWidgetService = GetGameService(self,GameServiceNameDef.GameWidgetService)
    if GameWidgetService then
        GameWidgetService:ShowWidget(GameWidgetNameDef.WidgetGenericMessageDialog, true, InMessage)
    end
end

return PopupMessageSubsystem
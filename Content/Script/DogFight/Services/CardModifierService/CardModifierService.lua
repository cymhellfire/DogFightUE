require "Common.StringHelper"

---@class CardModifierService : GameServiceBase, UCardModifierService
local CardModifierService = UnrealClass("DogFight.Services.GameServiceBase")

---@param InId number Id of card modifier to create
---@return UCardAttributeModifier New modifier
function CardModifierService:CreateCardModifierInternal(InId)
    -- Get config
    local Config = self.Config:GetConfig(InId)
    if not Config then
        return nil
    end

    -- Create modifier and bind script
    ---@type UCardAttributeModifier
    local NewModifier = NewObject(UE.UCardAttributeModifier.StaticClass(), self)

    -- Load and initialize with modifier script
    if NewModifier then
        NewModifier.ModifierType = UE.ELuaAttributeModifierType.AMT_CardAttribute
        NewModifier:LoadAndInitModifierScript(InId)
    end

    return NewModifier
end

function CardModifierService:GetConfigPath()
    return "DogFight.Services.CardModifierService.CardModifierConfig"
end

return CardModifierService
